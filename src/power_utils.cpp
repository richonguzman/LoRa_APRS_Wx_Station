/* Copyright (C) 2025 Ricardo Guzman - CA2RXU
 *
 * This file is part of LoRa APRS iGate.
 *
 * LoRa APRS iGate is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LoRa APRS iGate is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LoRa APRS iGate. If not, see <https://www.gnu.org/licenses/>.
 */

#include <Wire.h>
#include "configuration.h"
//#include "battery_utils.h"
#include "board_pinout.h"
#include "power_utils.h"
#include "utils.h"

extern Configuration    Config;
extern bool             stationCallsignIsValid;


namespace POWER_Utils {

    #ifdef ADC_CTRL_PIN
        void adc_ctrl_ON() {
            digitalWrite(ADC_CTRL_PIN, ADC_CTRL_ON_STATE);
        }

        void adc_ctrl_OFF() {
            digitalWrite(ADC_CTRL_PIN, !ADC_CTRL_ON_STATE);
        }
    #endif

    #ifdef VEXT_CTRL_PIN
        void vext_ctrl_ON() {
            digitalWrite(VEXT_CTRL_PIN, VEXT_CTRL_ON_STATE);
        }

        void vext_ctrl_OFF() {
            digitalWrite(VEXT_CTRL_PIN, !VEXT_CTRL_ON_STATE);
        }
    #endif

    void setup() {
        #ifdef BATTERY_PIN
            pinMode(BATTERY_PIN, INPUT);
        #endif

        #ifdef INTERNAL_LED_PIN
            pinMode(INTERNAL_LED_PIN, OUTPUT);
            digitalWrite(INTERNAL_LED_PIN, LOW);
        #endif

        #ifdef VEXT_CTRL_PIN
            pinMode(VEXT_CTRL_PIN,OUTPUT); // GPS + TFT on HELTEC Wireless_Tracker and only for Oled in HELTEC V3
            vext_ctrl_ON();
        #endif

        #ifdef ADC_CTRL_PIN
            pinMode(ADC_CTRL_PIN, OUTPUT);
            adc_ctrl_OFF();
        #endif

        #ifdef USE_WIRE_WITH_OLED_PINS
            Wire.begin(OLED_SDA, OLED_SCL);
        #endif

        #ifdef SENSOR_I2C_BUS
            SENSOR_I2C_BUS.begin(BOARD_I2C_SDA, BOARD_I2C_SCL);
        #endif

        /*delay(1000);
        BATTERY_Utils::setup();
        BATTERY_Utils::startupBatteryHealth();
        stationCallsignIsValid = Utils::callsignIsValid(Config.callsign);*/
        setCpuFrequencyMhz(80);
    }

}