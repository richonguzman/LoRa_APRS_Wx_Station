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

int     telemetryCounter                = random(1,999);


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

    String generateEncodedTelemetryBytes(float value, bool firstBytes, byte voltageType) {  // 0 = internal battery(0-4,2V) , 1 = external battery(0-15V)
        String encodedBytes;
        int tempValue;

        if (firstBytes) {
            tempValue = value;
        } else {
            switch (voltageType) {
                case 0:
                    tempValue = value * 100;           // Internal voltage calculation
                    break;
                case 1:
                    tempValue = (value * 100) / 2;     // External voltage calculation
                    break;
                default:
                    tempValue = value;
                    break;
            }
        }        

        int firstByte   = tempValue / 91;
        tempValue       -= firstByte * 91;

        encodedBytes    = char(firstByte + 33);
        encodedBytes    += char(tempValue + 33);
        return encodedBytes;
    }

    String generateEncodedTelemetry() {
        String telemetry = "|";
        telemetry += generateEncodedTelemetryBytes(telemetryCounter, true, 0);
        telemetryCounter++;
        if (telemetryCounter == 1000) {
            telemetryCounter = 0;
        }
        if (Config.battery.sendInternalVoltage) telemetry += generateEncodedTelemetryBytes(3.987/*checkInternalVoltage()*/, false, 0);
        if (Config.battery.sendExternalVoltage) telemetry += generateEncodedTelemetryBytes(12.123/*checkExternalVoltage()*/, false, 1);
        telemetry += "|";
        return telemetry;
    }
}