#include <RadioLib.h>
#include <SPI.h>
#include "configuration.h"
#include "boards_pinout.h"

extern Configuration    Config;

#ifdef HAS_SX1278
    SX1278 radio = new Module(RADIO_CS_PIN, RADIO_BUSY_PIN, RADIO_RST_PIN);
#endif

bool operationDone   = true;
bool transmitFlag    = true;

namespace LoRa_Utils {

    void setFlag(void) {
        operationDone = true;
    }

    void setup() {        
        //Serial.println("LoRa  Set SPI pins!");
        SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN);
        float freq = (float)Config.loramodule.txFreq / 1000000;
        int state = radio.begin(freq);
        if (state == RADIOLIB_ERR_NONE) {
            //Serial.println("Initializing LoRa Module");
        } else {
            Serial.println("Starting LoRa failed! State: " + String(state));
            while (true);
        }
        #if defined(HAS_SX1278)
            radio.setDio0Action(setFlag, RISING);
        #endif
        radio.setSpreadingFactor(Config.loramodule.spreadingFactor);
        float signalBandwidth = Config.loramodule.signalBandwidth/1000;
        radio.setBandwidth(signalBandwidth);
        radio.setCodingRate(Config.loramodule.codingRate4);
        radio.setCRC(true);
        state = radio.setOutputPower(Config.loramodule.power);
        radio.setCurrentLimit(100);
        if (state == RADIOLIB_ERR_NONE) {
            Serial.println("init : LoRa Module    ...     done!");
        } else {
            Serial.println("Starting LoRa failed! State: " + String(state));
            while (true);
        }
    }

    void sendNewPacket(const String& newPacket) {
        digitalWrite(LedPin, HIGH);
        int state = radio.transmit("\x3c\xff\x01" + newPacket);
        transmitFlag = true;
        if (state == RADIOLIB_ERR_NONE) {
            //Serial.print("---> LoRa Packet Tx    : ");
            //Serial.println(newPacket);
        } else {
            Serial.print(F("failed, code "));
            Serial.println(String(state));
        }
        digitalWrite(LedPin, LOW);
    }

}