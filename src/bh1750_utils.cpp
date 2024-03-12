#include "pins_config.h"
#include <bh1750_utils.h>
#include <BH1750.h> 

#define luminousEfficacy (112.0)  // Luminous efficacy for sunlight in lumens per watt

extern String Luminosity;
extern String seventhLine;

namespace BH1750_Utils {

    BH1750  lightMeter;

    void setup() {
        bool status;
        status = lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23, &Wire);
        if (!status) {
            Serial.println("Could not find a valid BH1750 sensor, check wiring!");
            while (1);
        } else {
            Serial.println("init : BH1750 Module  ...     done!");
        }
    }

    String generateLumString(float lum) {
        String light = ""; 
        light  = String((int)lum);
        switch (light.length()) {
            case 1:
                return "00" + light;
                break;
            case 2:
                return "0" + light;
                break;
            case 3:
                return light;
                break;
            default:
                return "...";
        }
    }

    void readSensor() {  //  BH1750 / GY-302 light sensor
        float lux = lightMeter.readLightLevel();
        if (lux > 40000) Serial.println("Lux > 40000!!!!");
        /*if (lux > 40000.0) {
            if (lightMeter.setMTreg(32)) {      // reduce measurement time - needed in direct sun light
                Serial.println("Setting MTReg to low value for high light environment");
            } else {
                Serial.println("Error setting MTReg to low value for high light environment");
            }
        } else {
            if (lux > 10.0) {
                if (lightMeter.setMTreg(69)) {  // typical light environment
                    Serial.println("Setting MTReg to default value for normal light environment");
                } else {
                    Serial.println("Error setting MTReg to default value for normal light environment");
                }
            } else {
                if (lux <= 10.0) {              // very low light environment
                    if (lightMeter.setMTreg(138)) {
                        Serial.println("Setting MTReg to high value for low light environment");
                    } else {
                        Serial.println("Error setting MTReg to high value for low light environment"));
                    }
                }
            }
        }*/
        Luminosity = generateLumString(lux/luminousEfficacy);
        seventhLine = "Luminosity : " + Luminosity + " W/m2";
    }    

}