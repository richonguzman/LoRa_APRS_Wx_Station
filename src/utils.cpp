#include <Wire.h>
#include "wind_rs485_utils.h"
#include "boards_pinout.h"
#include "utils.h"
#include "display.h"


uint8_t bh1750Addr  = 0x00;
uint8_t bme280Addr  = 0x00;


namespace Utils {

    void pinDeclarations() {
        pinMode(LedPin, OUTPUT);
        pinMode(rainSwitchPin,INPUT_PULLUP);
        pinMode(windInfoAddrSwitchPin, INPUT_PULLDOWN);
        pinMode(windChangeAddrSwitchPin, INPUT_PULLDOWN);
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
}