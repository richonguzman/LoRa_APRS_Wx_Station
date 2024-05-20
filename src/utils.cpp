#include <Wire.h>
#include "wind_rs485_utils.h"
#include "boards_pinout.h"
#include "utils.h"


uint8_t bh1750Addr  = 0x00;
uint8_t bme280Addr  = 0x00;


namespace Utils {

    void pinDeclarations() {
        pinMode(LedPin, OUTPUT);
        pinMode(rainSwitchPin,INPUT_PULLUP);
        pinMode(windInfoAddrSwitchPin, INPUT);
        pinMode(windChangeAddrSwitchPin, INPUT);
    }

    void checkWindDireccionSensorAddress() {
        if (digitalRead(windInfoAddrSwitchPin) == LOW && digitalRead(windChangeAddrSwitchPin) == HIGH) {
            delay(3000);
            Serial.println("Starting : RS485 Sensor Address Identifier...");
            WIND_RS485_Utils::setup();
            while(1) {
                WIND_RS485_Utils::checkSensorAddress();
                delay(4000);
            }
        } else {
            Serial.println("Pin14 is LOW, turn it off or disconnect it for Check Addresses");
        }
    }

    void changeWindDireccionSensorAddress() {
        if (digitalRead(windInfoAddrSwitchPin) == HIGH && digitalRead(windChangeAddrSwitchPin) == LOW) {
            delay(3000);     
            Serial.println("RS485  Sensor address change procedure.");
            WIND_RS485_Utils::setup();
            WIND_RS485_Utils::changeSensorAddress();
        } else {
            Serial.println("Pin13 is LOW, turn it off or disconnect it for Changing Address");
        }
    }

    bool checkSwitchesStates() {
        if (digitalRead(windInfoAddrSwitchPin) == HIGH && digitalRead(windChangeAddrSwitchPin) == HIGH) {
            return true;
        } else {
            return false;
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