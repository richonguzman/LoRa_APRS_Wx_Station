#include "ml8511_utils.h"
#include "boards_pinout.h"


namespace ML8511_Utils {

    float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    void readSensor() {
        int sensorReadings = 10;
        digitalWrite(UV_EN_PIN, HIGH);
        delay(100);

        int sample;
        int sampleSum = 0;
        for (int i = 0; i < sensorReadings; i++) {
            sample = 0;
            sample = analogRead(UV_OUT_PIN);
            sampleSum += sample;
            delayMicroseconds(50); 
        }
        sample  = sampleSum/sensorReadings;
        float outputVoltage = sample * (3.3 / 4096.0);

        float uvIntensity = mapfloat(outputVoltage, 0.89, 2.9, 0.0, 15.0);
        Serial.print("MP8511 output: ");
        Serial.println(sample);
        Serial.print("MP8511 voltage: ");
        Serial.println(outputVoltage);
        Serial.print("UV Intensity (mW/cm^2): ");
        Serial.println(uvIntensity);

        digitalWrite(UV_EN_PIN, LOW);
    }

}