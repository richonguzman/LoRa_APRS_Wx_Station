#include "boards_pinout.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <bme280_utils.h>
#include "configuration.h"
#include "display.h"
#include "utils.h"

#define SEALEVELPRESSURE_HPA (1013.25)
#define heightCorrectionFactor (8.2296)      // for meters


extern Configuration    Config;

/*********** TO BE ADDED FROM CONFIGURATION ***********/
//int         heightCorrection        = 0;
//float       temperatureCorrection   = 0.0;
/******************************************************/

extern String   secondLine;
extern String   thirdLine;
extern String   fourthLine;

extern uint8_t  bme280Addr;

bool    bme280SensorFound   = false;
String  Temperature         = "...";
String  Humidity            = "..";
String  BarometricPressure  = ".....";


namespace BME280_Utils {

    Adafruit_BME280 bme280;

    void setup() {
        if (bme280Addr != 0x00) {
            bool status;
            status = bme280.begin(bme280Addr);
            if (!status) {
                Serial.println("Could not initialize BME280 , check sensor!");
                displayShow("ERROR", "", "BME280 found but", "could not init ...", 2000);
            } else {
                bme280.setSampling(Adafruit_BME280::MODE_FORCED,
                                Adafruit_BME280::SAMPLING_X1,
                                Adafruit_BME280::SAMPLING_X1,
                                Adafruit_BME280::SAMPLING_X1,
                                Adafruit_BME280::FILTER_OFF
                                );
                Serial.println("init : BME280 Module  ...     done!");
                bme280SensorFound = true;
            }
        } else {
            Serial.println("Could not find a BME280 sensor, check wiring!");
            displayShow("ERROR", "", "BME280 NOT FOUND !!!", "", 2000);
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
        String strPress = String((int)bmePress);
        String decPress = String(int((bmePress - int(bmePress)) * 10));
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
        bme280.takeForcedMeasurement();
        newTemp   = bme280.readTemperature();
        newPress  = (bme280.readPressure() / 100.0F);
        newHum = bme280.readHumidity();
        if (isnan(newTemp) || isnan(newHum) || isnan(newPress)) {
            Serial.println("BME280 Module data failed");
            Temperature         = "...";
            Humidity            = "..";
            BarometricPressure  = ".....";
        } else {
            Temperature         = generateTempString(((newTemp + Config.sensors.bem280TemperatureCorrection) * 1.8) + 32);
            Humidity            = generateHumString(newHum);
            BarometricPressure  = generatePresString(newPress + (Config.sensors.bme280HeightCorrection/heightCorrectionFactor));
        }
        secondLine  = "Temp       : ";
        secondLine  += Temperature;
        secondLine  += " F";

        thirdLine   = "Humidity   :  ";
        thirdLine   += Humidity;
        thirdLine   += " %";

        fourthLine  = "Pressure   : ";
        fourthLine  += BarometricPressure.substring(0,BarometricPressure.length() - 1);
        fourthLine  += " hPA";
    }

}