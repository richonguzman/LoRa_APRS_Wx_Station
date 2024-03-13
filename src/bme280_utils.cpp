#include "pins_config.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <bme280_utils.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define heightCorrectionFactor (8.2296)      // for meters

//  CONFIGURATION
#define heightCorrection 0       // in meters
//

extern String Temperature;
extern String Humidity;
extern String BarometricPressure;
extern String secondLine;
extern String thirdLine;
extern String fourthLine;


namespace BME280_Utils {

    Adafruit_BME280 bme;

    void setup() {
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

    void readSensor() {
        float newHum, newTemp, newPress;
        bme.takeForcedMeasurement();
        newTemp   = bme.readTemperature();
        newPress  = (bme.readPressure() / 100.0F);
        newHum = bme.readHumidity();
        if (isnan(newTemp) || isnan(newHum) || isnan(newPress)) {
            Serial.println("BME/BMP Module data failed");
            Temperature         = "...";
            Humidity            = "..";
            BarometricPressure  = ".....";
        } else {
            Temperature         = generateTempString((newTemp * 1.8) + 32);
            Humidity            = generateHumString(newHum);
            BarometricPressure  = generatePresString(newPress + (heightCorrection/heightCorrectionFactor));
        }
        secondLine  = "Temp     : " + Temperature + " F";
        thirdLine   = "Humidity : " + Humidity + " %";
        fourthLine  = "Pressure : " + BarometricPressure.substring(0,BarometricPressure.length()-1) + " hPA";
    }

}