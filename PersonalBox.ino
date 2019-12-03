/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP8266 chip.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!2
 *************************************************************/

/* Comment this out to disable prints and save space */
// #define BLYNK_PRINT Serial

#include <WiFiManager.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Stepper.h>

char auth[] = "a7df3e39329044618814d1d9f8a906d2";
bool pinValueV0; // Boolean to be written to check if V0 is 1 or 0
const int stepsPerRevolution = 512; // Aantal stappen (kwart)
Stepper stepper(stepsPerRevolution, D5, D6, D7,  D8);

// Openen / Sluiten van solonoid
BLYNK_WRITE(V0){ 
  int pinValue = param.asInt();
  if (pinValue == 1) {
    digitalWrite(D2, HIGH); // Open solonoid
  }
  else {
    digitalWrite(D2, LOW); //Sluit Solonoid
  }
}

// Combinatie van open gaan en openen van solonoid, levert echter vaak een reset op
BLYNK_WRITE(V1){
  int pinValue = param.asInt();
  
  if (pinValue == 1) {
    digitalWrite(D2, HIGH); // Open solonoid
    stepper.step(stepsPerRevolution); // Open
    digitalWrite(D2, LOW); //Sluit Solonoid
  } 
  else {
    digitalWrite(D2, HIGH); // Open solonoid
    stepper.step(-stepsPerRevolution); // Sluit 
    digitalWrite(D2, LOW); //Sluit Solonoid
    // Pinnen op low zodat motor niet heet wordt
    digitalWrite(D5, LOW); 
    digitalWrite(D6, LOW);
    digitalWrite(D7, LOW);
    digitalWrite(D8, LOW);}
}

// Openen van de kist met de stappen motor
BLYNK_WRITE(V2){
  int pinValue = param.asInt();
  if (pinValue == 1) {
    stepper.step(stepsPerRevolution); // Open
    digitalWrite(D2, LOW); //Sluit Solonoid
    Blynk.virtualWrite(V0, 0); 
  }
  else {
    stepper.step(-stepsPerRevolution); // Sluit 
    Blynk.virtualWrite(V0, 0);
    // Pinnen op low zodat motor niet heet wordt
    digitalWrite(D5, LOW); 
    digitalWrite(D6, LOW);
    digitalWrite(D7, LOW);
    digitalWrite(D8, LOW);}
}

void setup(){
  pinMode(D2, OUTPUT); // Zet pin D2 (solonoid) op out
  // Debug console
  // Serial.begin(9600);
  WiFiManager wifiManager;
  
  //If he can find a wifi network that is already configured it will go automaticly
  //Otherwise you have to set it up in your wifi settings, it will make an accesspoint
  wifiManager.autoConnect("ESP-Jesper");
  Blynk.config(auth);
  Blynk.connect();
  stepper.setSpeed(60);            //Snelheid
}

void loop(){
  Blynk.run();
}
