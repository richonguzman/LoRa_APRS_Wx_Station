#include "wx_utils.h"
#include "pins_config.h"
#include "lora_utils.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define CORRECTION_FACTOR (8.2296)      // for meters

extern uint32_t     lastBeaconTx;
extern int          beaconInterval;
extern bool         beaconUpdate;

//
#define HEIGHT_CORRECTION 0             // in meters
extern String       callsign;
extern String       tocall;     // fijo?
extern String       path;
extern String       overlay;    // fijo?
extern String       symbol;     // fijo?
extern String       comment;
//

String Temperature, Humidity, BarometricPressure, Luminosity, WindDirection, WindSpeed, Gust, RainLastHr, RainLast24Hr;

namespace WX_Utils {

    Adafruit_BME280   bme;

    void setupBME() {
        bool status;
        status = bme.begin(0x76);  // Don't forget to join pins for righ direction on BME280!
        if (!status) {
            Serial.println("Could not find a valid BME280 or BMP280 sensor, check wiring!");
            while (1); // sacar esto para que quede pegado si no encuentra BME280
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

    void readSensor_BME280() {
        /*float newHum, newTemp, newPress;
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
            BarometricPressure  = generatePresString(newPress + (HEIGHT_CORRECTION/CORRECTION_FACTOR));
        }*/
        //
        Temperature          = "63";        // fahrenheit (bellow 0 ... -01 or -99)
        Humidity             = "60";        // %
        BarometricPressure   = "10212";     // hPa
        //
    }

    void readSensor_Luminosity() {
        Luminosity           = "840";       // "L" si es menor que 1000 W/m2 y "l" si es >= 1000 W/m2 y reemplaza algunos de los campos de lluvia.
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
        readSensor_Luminosity();
        readSensor_Wind();
        String wxPacket = WindDirection + "/" + WindSpeed + "g" + Gust + "t" + Temperature + "r" + RainLastHr + "p" + RainLast24Hr + "L" + Luminosity +"h" + Humidity + "b" + BarometricPressure;
        
        return callsign + ">" + tocall + "," + path + ":=" + datoGPSLat + overlay + datoGPSLon + symbol + wxPacket + comment;
    }


    void loop() {
        readSensor_Rain();
        uint32_t lastTx = millis() - lastBeaconTx;
        if (lastTx >= beaconInterval*60*1000) {
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

}