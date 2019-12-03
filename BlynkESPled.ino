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
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
// #define BLYNK_PRINT Serial

#include <ESP8266HTTPClient.h>
#include <WiFiManager.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"
#define LED_PIN     D6 //ORNG
#define NUM_LEDS    239
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS];
int i;

bool pinData; // Geef dataPin low waarde, wordt gesynced later

#define BUTTON_PIN D1 //Pin button

CRGB convertToGRB(CRGB rgb){
    return CRGB(rgb.g,rgb.r,rgb.b);}

char auth[] = "20bcb9212ce74f5c920aea239e416289";

BlynkTimer timer; //Define timer

BLYNK_WRITE(V1) //Button Widget is writing to pin V1
{
  int pinData = param.asInt(); 
  if (pinData == 1){
    HTTPClient http; //Declare an object of class HTTPClient
    http.begin("http://192.168.0.112/cm?cmnd=Power%20On"); //Specify request destination
    http.GET();
    http.end();
    pinData = 1;
  }
  else{
    HTTPClient http; //Declare an object of class HTTPClient
    http.begin("http://192.168.0.112/cm?cmnd=Power%20Off"); //Specify request destination
    http.GET();
    http.end();
    pinData = 0;
  }
}

BLYNK_WRITE(V2) {
  switch (param.asInt())
  {
    case 1: // Blauw
      for( int i = 0; i < 136; i++){
        leds[i] = CRGB(0, 0, 255);
        leds2[i] = leds[i];
        FastLED.show();}
      for( int i = 137; i < 239; i++){
        leds[i] = CRGB(0, 0, 255);
        leds2[i] = convertToGRB(leds[i]);
        FastLED.show();}
      break;
      
    case 2: // Rood
      for( int i = 0; i < 136; i++){
        leds[i] = CRGB(255, 0, 0);
        leds2[i] = leds[i];
        FastLED.show();}
      for( int i = 137; i < 239; i++){
        leds[i] = CRGB(255, 0, 0);
        leds2[i] = convertToGRB(leds[i]);
        FastLED.show();}
      break;
      
    case 3: // Paars
        for( int i = 0; i < 136; i++){
        leds[i] = CRGB(255, 0, 255);
        leds2[i] = leds[i];
        FastLED.show();}
      for( int i = 137; i < 239; i++){
        leds[i] = CRGB(255, 0, 255);
        leds2[i] = convertToGRB(leds[i]);
        FastLED.show();}
      break;

    case 4: // Meerkleuren
        for( int i = 0; i < 29; i++) //Bureau onder blauw
      {   leds[i] = CRGB(0, 0, 255);
          leds2[i] = leds[i];
          FastLED.show();}
      for( int i = 30; i < 59; i++) //Bureau achter rood
      {   leds[i] = CRGB(255, 0, 0);
          leds2[i] = leds[i];
          FastLED.show();}
      for( int i = 60; i < 89; i++) //Bed groen
      {   leds[i] = CRGB(0, 255, 0);
          leds2[i] = leds[i];
          FastLED.show();}
      for( int i = 90; i < 118; i++) //Kast groen
          {   leds[i] = CRGB(0, 255, 0);
          leds2[i] = leds[i];
          FastLED.show();}
      for( int i = 119; i < 136; i++) //Balk 1 onder blauw
      {   leds[i] = CRGB(0, 0, 255);
          leds2[i] = leds[i];
          FastLED.show();}
      for( int i = 137; i < 150; i++) //Balk 1 onder 2e gedeelte blauw
      {   leds[i] = CRGB(0, 0, 255);
          leds2[i] = convertToGRB(leds[i]);
          FastLED.show();}
      for( int i = 151; i < 176; i++) //Balk 1 boven rood
      {   leds[i] = CRGB(255, 0, 0);
          leds2[i] = convertToGRB(leds[i]);
          FastLED.show();}
      for( int i = 177; i < 211; i++) //Balk 2 onder blauw
      {   leds[i] = CRGB(0, 0, 255);
          leds2[i] = convertToGRB(leds[i]);
          FastLED.show();}   
      for( int i = 212; i < 239; i++) //Balk 2 boven rood
      {   leds[i] = CRGB(255, 0, 0);
          leds2[i] = convertToGRB(leds[i]);
          FastLED.show();}
    break;
    
    default:
    break;
  }
}

void checkbutton(){
  if (digitalRead(BUTTON_PIN) == LOW){ // Low means pressed
    if (pinData == 0){
      HTTPClient http; //Declare an object of class HTTPClient
      http.begin("http://192.168.0.112/cm?cmnd=Power%20On"); //Specify request destination
      http.GET();
      http.end();
      Blynk.virtualWrite(V1, HIGH);
      pinData = 1;
    }
    else {
      HTTPClient http; //Declare an object of class HTTPClient
      http.begin("http://192.168.0.112/cm?cmnd=Power%20Off"); //Specify request destination
      http.GET();
      http.end();
      Blynk.virtualWrite(V1, LOW);
      pinData = 0;
    }
  }
}

void setup()
{
  // Set pin mode on pullup for button
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Debug console
  // Serial.begin(9600);
  // WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFiManager wifiManager;
  WiFiManagerParameter custom_blynk_token("Blynk", "blynk token", auth, 34);
  wifiManager.addParameter(&custom_blynk_token);
  wifiManager.autoConnect("ESP-Jesper");
  Blynk.config(custom_blynk_token.getValue());
  Blynk.connect();
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds2, NUM_LEDS).setCorrection(TypicalLEDStrip); 
  FastLED.setBrightness(  BRIGHTNESS );
  
  // Setup a function to be called every 100 ms
  timer.setInterval(250L, checkbutton);

  Blynk.syncVirtual(V1);
}


void loop()
{
  Blynk.run();
  timer.run();
}
