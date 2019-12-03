/*
  Lora Send And Receive
  This sketch demonstrates how to send and receive data with the MKR WAN 1300 LoRa module.
  This example code is in the public domain.
*/

#include <MKRWAN.h>
#include "DHT.h"
LoRaModem modem;
#include "arduino_secrets.h"
// Please enter your sensitive data in the Secret tab or arduino_secrets.h
String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;

#define DHTPIN 0 //Digital pin for temperature/humidity
#define WTRPIN1 A1 //Analog pin for moisture sensor 1
#define WTRPIN2 A2 //Analog pin for moisture sensor 2
#define pomp1 1
#define pomp2 2
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);

void setup() {

  pinMode(pomp1, OUTPUT);
  pinMode(pomp2, OUTPUT);
  
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);
  // change this to your regional band (eg. US915, AS923, ...)
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1) {}
  };
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());

  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    while (1) {}
  }


  // Set poll interval to 60 secs.
  modem.minPollInterval(60);
  // NOTE: independently by this setting the modem will
  // not allow to send more than one message every 2 minutes,
  // this is enforced by firmware and can not be changed.
  
  String msg = "Ready";
  Serial.println("Sending...: " + msg);

  int err;
  modem.beginPacket();
  modem.print(msg);
  delay(5000);
  err = modem.endPacket(true);
  if (err > 0) {
    Serial.println("Message sent correctly!");
  } else {
    Serial.println("Error sending message :(");
  }   //Connect message just once

}

void loop() {
  float w1 = analogRead(WTRPIN1);
  float w2 = analogRead(WTRPIN2);

  if(w1 < 600){
    digitalWrite(pomp1, HIGH);
    delay(5000);
    digitalWrite(pomp1, LOW);
    String msg = "P1 watered";
    msgwatered(msg);
    delay(300000);
  }
  else if(w2 < 600){
    digitalWrite(pomp2, HIGH);
    delay(5000);
    digitalWrite(pomp2, LOW);
    String msg = "P2 watered";
    msgwatered(msg);
    delay(300000);    
  }
  else{
    delay(300000);
  }
  
  msr();

}

void msgwatered(String msg){
  Serial.println("Sending...: " + msg);

  int err;
  modem.beginPacket();
  modem.print(msg);
  delay(5000);
  err = modem.endPacket(true);
  if (err > 0) {
    Serial.println("Message sent correctly!");
  } else {
    Serial.println("Error sending message :(");
  }   //Connect message just once
}

void msr(){
  byte payload[4];
  
  Serial.println("In the function");
  
  delay(1000); //Measurements can be done every 2 seconds
  float h = dht.readHumidity(); //Read humidity
  payload[0] = h;
  
  float t = dht.readTemperature(); //Read temperature
  payload[1] = t;
  
  int w1 = analogRead(WTRPIN1);
  Serial.print("Water 1 zonder bytes: ");
  Serial.println(w1);
  payload[2] = (w1 / 10);
  
  int w2 = analogRead(WTRPIN2);
  Serial.print("Water 2 zonder bytes: ");
  Serial.println(w2);
  payload[3] = (w2 / 10);  


  float f_h = payload[0];
  Serial.print("Humidty: ");
  Serial.println(f_h);
  float f_t = payload[1];
  Serial.print("Temperature: ");
  Serial.println(f_t);
  float f_w1 = payload[2];
  Serial.print("Water 1: ");
  Serial.println(f_w1);
  float f_w2 = payload[3];
  Serial.print("Water 2: ");
  Serial.println(f_w2);

  Serial.println("Sending...: ");
  
  int err;
  modem.beginPacket();
  modem.write(payload, sizeof(payload));
  err = modem.endPacket(true);
  if (err > 0) {
    Serial.println("Message sent correctly!");
  } else {
    Serial.println("Error sending message :(");
  }
}
