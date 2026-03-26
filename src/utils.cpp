#include <Wire.h>
#include "board_pinout.h"
#include "utils.h"


uint8_t bh1750Addr  = 0x00;
uint8_t bme280Addr  = 0x00;


namespace Utils {

    void pinDeclarations() {
        pinMode(INTERNAL_LED_PIN, OUTPUT);
        #ifdef RAIN_SWITCH_PIN
            pinMode(RAIN_SWITCH_PIN,INPUT_PULLUP);
        #endif
        delay(500);
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
