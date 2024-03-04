#include <LoRa.h>
#include <SPI.h>
#include "pins_config.h"

long freq = 433775000;
int signalBandwith = 125000;
int spreadingFactor = 12;
int codingRate = 5;
int power = 20;

namespace LoRa_Utils {

    void setup() {        
        Serial.println("LoRa   Set SPI pins!");
        SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
        LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);
        if (!LoRa.begin(freq)) {
            Serial.println("LoRa   Starting LoRa failed!");
            while (true) {
                delay(1000);
            }
        }
        LoRa.setSpreadingFactor(spreadingFactor);
        LoRa.setSignalBandwidth(signalBandwith);
        LoRa.setCodingRate4(codingRate);
        LoRa.enableCrc();
        LoRa.setTxPower(power);
        Serial.println("LoRa   LoRa init done!");
        String currentLoRainfo = "LoRa Freq: " + String(freq)  + " / SF:" + String(spreadingFactor) + " / CR: " + String(codingRate);
        Serial.print("LoRa  "); Serial.println(currentLoRainfo);   
    }

    void sendNewPacket(const String &newPacket) {
        LoRa.beginPacket();
        LoRa.write('<');
        LoRa.write(0xFF);
        LoRa.write(0x01);
        LoRa.write((const uint8_t *)newPacket.c_str(), newPacket.length());
        LoRa.endPacket();
    }

}