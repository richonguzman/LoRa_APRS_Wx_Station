#include <Arduino.h>
#include "battery_utils.h"
#include "configuration.h"
#include "boards_pinout.h"
#include "utils.h"

extern  Configuration                   Config;
extern  uint32_t                        lastBatteryCheck;

float   adcReadingTransformation        = (3.3/4095);
float   voltageDividerCorrection        = 0.288;
float   readingCorrection               = 0.125;
float   multiplyCorrection              = 0.035;

float   voltageDividerTransformation    = 0.0;


namespace BATTERY_Utils {

    float checkInternalVoltage() { 
        int sample      = 0;
        int sampleSum   = 0;
        for (int i = 0; i < 100; i++) {
            #ifdef BATTERY_PIN
                sample = analogRead(BATTERY_PIN);
            #else
                sample = 0;
            #endif
            sampleSum += sample;
            delayMicroseconds(50); 
        }
        return (2 * (sampleSum/100) * adcReadingTransformation) + voltageDividerCorrection;        
    }

    float checkExternalVoltage() {
        int sample;
        int sampleSum = 0;
        for (int i = 0; i < 100; i++) {
            sample = analogRead(35);        //Config.battery.externalVoltagePin);
            sampleSum += sample;
            delayMicroseconds(50);
        }
        float extVoltage = ((((sampleSum/100)* adcReadingTransformation) + readingCorrection) * voltageDividerTransformation) - multiplyCorrection;        
        return extVoltage; // raw voltage without mapping
    }

}