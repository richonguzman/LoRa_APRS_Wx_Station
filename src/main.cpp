#include <Arduino.h>
#include <TinyGPS++.h>
#include "lora_utils.h"
#include "gps_utils.h"
#include "utils.h"

HardwareSerial  neo6m_gps(1);
TinyGPSPlus     gps;

#define LED 8
int contador = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  delay(4000);
  LoRa_Utils::setup();
  GPS_Utils::setup();
}

void loop() {
  digitalWrite(LED, HIGH);
  Serial.println("LED is on");
  delay(1000);
  digitalWrite(LED, LOW);
  Serial.println("LED is off");
  delay(1000);

  /*contador++;
  String packet = "AB1CDE-7>APLRT1,WIDE1-1::CA2RXU-7 :testMessage " + String(contador);
  LoRa_Utils::sendNewPacket(packet);
  delay(15000);*/

  GPS_Utils::getData();
  GPS_Utils::setDateFromData();

  const auto time_now = now();
  Serial.println(Utils::createDateString(time_now) + "   " + Utils::createTimeString(time_now));

  

}