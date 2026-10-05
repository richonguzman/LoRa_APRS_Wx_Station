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

#if defined(HAS_SX1278) || defined(HAS_SX1276)
    #define CHIP_MIN_POWER  2       // PA_BOOST (below 2 RadioLib switches to RFO, not wired on most modules)
    #define CHIP_MAX_POWER  20
#else                               // SX1262 / SX1268 / LLCC68
    #define CHIP_MIN_POWER  -9
    #define CHIP_MAX_POWER  22
#endif

#ifndef RADIO_MAX_POWER             // optional per board in boards_pinout.h (e.g. 1W PA modules)
    #define RADIO_MAX_POWER CHIP_MAX_POWER
#endif

namespace LoRa_Utils {

    void setFlag(void) {
        operationDone = true;
    }

    int validPower(int requested) {
        const int maxPower = (RADIO_MAX_POWER < CHIP_MAX_POWER) ? RADIO_MAX_POWER : CHIP_MAX_POWER;
        int power = constrain(requested, CHIP_MIN_POWER, maxPower);
        #if defined(HAS_SX1278) || defined(HAS_SX1276)
            if (power > 17 && power < 20) power = 17;   // SX127x PA_BOOST: only 2-17 or 20
        #endif
        return power;
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
        int power = validPower(Config.loramodule.power);
        if (power != Config.loramodule.power) {
            Serial.println("LoRa power adjusted: " + String(Config.loramodule.power) + " -> " + String(power));
        }
        state = radio.setOutputPower(power);
        radio.setCurrentLimit(120);     // OCP ceiling for SX127x: ~120mA needed at +20dBm (not a fixed consumption)
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