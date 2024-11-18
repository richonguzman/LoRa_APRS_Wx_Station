#include <Wire.h>
#include "wind_rs485_utils.h"
#include "configuration.h"
#include "boards_pinout.h"
#include "utils.h"
#include "display.h"


uint8_t bh1750Addr  = 0x00;
uint8_t bme280Addr  = 0x00;

extern Configuration    Config;


namespace Utils {

    void pinDeclarations() {
        pinMode(LedPin, OUTPUT);

        if (Config.sensors.rainActive) pinMode(rainSwitchPin,INPUT_PULLUP);

        pinMode(windInfoAddrSwitchPin, INPUT_PULLDOWN);
        pinMode(windChangeAddrSwitchPin, INPUT_PULLDOWN);

        if (Config.sensors.ml8511) {
            pinMode(UV_EN_PIN, OUTPUT);
            digitalWrite(UV_EN_PIN, LOW);
            pinMode(UV_OUT_PIN, INPUT);
        }
        delay(500);
    }

    void checkWindDireccionSensorAddress() {
        displayShow("SENSOR ID", "Starting :", "RS485 Wind Direction", "Sensor Address", "Identifier...", "check -->", "Serial output");
        Serial.println("Starting : RS485 'Wind Direction' Sensor Address Identifier...");
        WIND_RS485_Utils::setup();
        delay(1000);
        while(1) {
            WIND_RS485_Utils::checkSensorAddress();
            delay(4000);
        }
    }

    void changeWindDireccionSensorAddress() {
        displayShow("SENSOR ID", "Starting :", "RS485 Wind Direction", "Sensor Address", "Change Procedure...", "check -->", "Serial output");   
        Serial.println("RS485  Sensor address change procedure.");
        WIND_RS485_Utils::setup();
        delay(1000);
        WIND_RS485_Utils::changeSensorAddress();
    }

    void checkSwitchesStates() {
        if (digitalRead(windInfoAddrSwitchPin) == HIGH && digitalRead(windChangeAddrSwitchPin) == LOW) {
            checkWindDireccionSensorAddress();
        } else if (digitalRead(windInfoAddrSwitchPin) == LOW && digitalRead(windChangeAddrSwitchPin) == HIGH) {
            changeWindDireccionSensorAddress();
        }
    }

    void getI2CAddresses() {
        uint8_t err, addr;
        for(addr = 1; addr < 0x7F; addr++) {
            Wire.beginTransmission(addr);
            err = Wire.endTransmission();
            if (err == 0) {
                //Serial.println(addr); this shows any connected board to I2C
                if (addr == 0x23) {
                    bh1750Addr = addr;
                } else if (addr == 0x76 || addr == 0x77) {
                    bme280Addr = addr;
                }
            }
        }
    }

    bool checkValidCallsign(const String& callsign) {
        if (callsign == "WLNK-1") return true;
        
        String cleanCallsign;
        if (callsign.indexOf("-") > 0) {    // SSID Validation
            cleanCallsign = callsign.substring(0, callsign.indexOf("-"));
            String ssid = callsign.substring(callsign.indexOf("-") + 1);
            if (ssid.indexOf("-") != -1 || ssid.length() > 2) return false;
            for (int i = 0; i < ssid.length(); i++) {
                if (!isAlphaNumeric(ssid[i])) return false;
            }
        } else {
            cleanCallsign = callsign;
        }

        if (cleanCallsign.length() < 4 || cleanCallsign.length() > 6) return false;

        if (cleanCallsign.length() < 6 && isAlpha(cleanCallsign[0]) && isDigit(cleanCallsign[1]) && isAlpha(cleanCallsign[2]) && isAlpha(cleanCallsign[3]) ) {
            cleanCallsign = " " + cleanCallsign;    // A0AA --> _A0AA
        }

        if (!isDigit(cleanCallsign[2]) || !isAlpha(cleanCallsign[3])) {     // __0A__ must be validated
            if (cleanCallsign[0] != 'R' && !isDigit(cleanCallsign[1]) && !isAlpha(cleanCallsign[2])) return false;    // to accepto R0A___
        }

        bool isValid = false;
        if ((isAlphaNumeric(cleanCallsign[0]) || cleanCallsign[0] == ' ') && isAlpha(cleanCallsign[1])) {
            isValid = true;     //  AA0A (+A+A) + _A0AA (+A) + 0A0A (+A+A)
        } else if (isAlpha(cleanCallsign[0]) && isDigit(cleanCallsign[1])) {
            isValid = true;     //  A00A (+A+A)
        } else if (cleanCallsign[0] == 'R' && cleanCallsign.length() == 6 && isDigit(cleanCallsign[1]) && isAlpha(cleanCallsign[2]) && isAlpha(cleanCallsign[3]) && isAlpha(cleanCallsign[4])) {
            isValid = true;     //  R0AA (+A+A)
        }
        if (!isValid) return false;   // also 00__ avoided

        if (cleanCallsign.length() > 4) {   // to validate ____AA
            for (int i = 5; i <= cleanCallsign.length(); i++) {
                if (!isAlpha(cleanCallsign[i - 1])) return false;
            }
        }
        return true;
    }

}