#include "wx_utils.h"
#include "pins_config.h"
#include "lora_utils.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <BH1750.h>
#include <Wire.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define heightCorrectionFactor (8.2296)      // for meters

#define luminousEfficacy (112.0)  // Luminous efficacy for sunlight in lumens per watt

extern uint32_t     lastBeaconTx;
extern int          beaconInterval;
extern bool         beaconUpdate;

//
#define heightCorrection 0       // in meters
extern String       callsign;
extern String       tocall;     // fijo?
extern String       path;
extern String       overlay;    // fijo?
extern String       symbol;     // fijo?
extern String       comment;
//

String Temperature, Humidity, BarometricPressure, Luminosity, WindDirection, WindSpeed, Gust, RainLastHr, RainLast24Hr;

namespace WX_Utils {

    Adafruit_BME280     bme;
    BH1750              lightMeter;

    void setupBME280() {
        bool status;
        status = bme.begin(0x76);
        if (!status) {
            Serial.println("Could not find a valid BME280 , check wiring!");
            while (1);
        } else {
            bme.setSampling(Adafruit_BME280::MODE_FORCED,
                            Adafruit_BME280::SAMPLING_X1,
                            Adafruit_BME280::SAMPLING_X1,
                            Adafruit_BME280::SAMPLING_X1,
                            Adafruit_BME280::FILTER_OFF
                            );
            Serial.println("init : BME280 Module  ...     done!");
        }
    }

    void setupBH1750() {
        bool status;
        status = lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23, &Wire);
        if (!status) {
            Serial.println("Could not find a valid BH1750 sensor, check wiring!");
            while (1);
        } else {
            Serial.println("init : BH1750 Module  ...     done!");
        }
    }

    String generateTempString(float bmeTemp) {
        String strTemp;
        strTemp = String((int)bmeTemp);
        switch (strTemp.length()) {
            case 1:
                return "00" + strTemp;
                break;
            case 2:
                return "0" + strTemp;
                break;
            case 3:
                return strTemp;
                break;
            default:
                return "-999";
        }
    }

    String generateHumString(float bmeHum) {
        String strHum;
        strHum = String((int)bmeHum);
        switch (strHum.length()) {
            case 1:
                return "0" + strHum;
                break;
            case 2:
                return strHum;
                break;
            case 3:
                if ((int)bmeHum == 100) {
                return "00";
                } else {
                return "-99";
                }
                break;
            default:
                return "-99";
        }
    }

    String generatePresString(float bmePress) {
        String strPress;
        strPress = String((int)bmePress);
        switch (strPress.length()) {
            case 1:
                return "000" + strPress + "0";
                break;
            case 2:
                return "00" + strPress + "0";
                break;
            case 3:
                return "0" + strPress + "0";
                break;
            case 4:
                return strPress + "0";
                break;
            case 5:
                return strPress;
                break;
            default:
                return "-99999";
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

    void readSensor_BME280() {
        float newHum, newTemp, newPress;
        bme.takeForcedMeasurement();
        newTemp   = bme.readTemperature();
        newPress  = (bme.readPressure() / 100.0F);
        newHum = bme.readHumidity();
        if (isnan(newTemp) || isnan(newHum) || isnan(newPress)) {
            Serial.println("BME/BMP Module data failed");
            // que se hace?
        } else {
            Temperature         = generateTempString((newTemp * 1.8) + 32);
            Humidity            = generateHumString(newHum);
            BarometricPressure  = generatePresString(newPress + (heightCorrection/heightCorrectionFactor));
        }
    }

    void readSensor_Luminosity() {  //  BH1750 / GY-302 light sensor
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
    }

    void readSensor_WindSpeed() {
        WindSpeed            = "002";    // mph
    }

    void readSensor_WindDirection() {
        WindDirection        = "123";    // degrees
    }

    void readSensor_Wind() {
        readSensor_WindDirection();
        readSensor_WindSpeed();
        //  calcular wind gust!
        Gust                 = "003";    // peak wind speed in last 5 min.
    }

    void readSensor_Rain() {
        //Serial.println("monitoreando sensor lluvia");
        //lluvia : ultimas 24hrs, ultima hr
        RainLastHr           = "004";    // hundreths of inch
        RainLast24Hr         = "020";    // hundreths of inch
    }

    String buildWxStationPacket() {
        String datoGPSLat = "3301.95S";     // reemplazar!
        String datoGPSLon = "07134.25W";    // reemplazar!

        readSensor_BME280();
        readSensor_Luminosity();           // "L" si es menor que 1000 W/m2 y "l" si es >= 1000 W/m2 y reemplaza algunos de los campos de lluvia.

        readSensor_Wind();
        String wxPacket = WindDirection + "/" + WindSpeed + "g" + Gust + "t" + Temperature + "r" + RainLastHr + "p" + RainLast24Hr + "L" + Luminosity +"h" + Humidity + "b" + BarometricPressure;
        
        return callsign + ">" + tocall + "," + path + ":=" + datoGPSLat + overlay + datoGPSLon + symbol + wxPacket + comment;
    }

    void loop() {
        readSensor_Rain();
        uint32_t lastTx = millis() - lastBeaconTx;
        if (lastTx >= beaconInterval*60*1000) {
        //if (lastTx >= beaconInterval*5*1000) {
            beaconUpdate = true;    
        }
        if (beaconUpdate) {            
            String wxPacket = buildWxStationPacket();
            LoRa_Utils::sendNewPacket(wxPacket);
            Serial.println("Enviando packet ---> " + wxPacket);
            lastBeaconTx = millis();
            beaconUpdate = false;
        }
        /*if (statusAfterBoot) {
            processStatus();
        }*/
    }

    void setupSensors() {
        Wire.begin();       //Wire.begin(OLED_SDA, OLED_SCL);
        Serial.println("\nSensors INI...");
        setupBME280();
        setupBH1750();
        Serial.println("\n");
    }

}