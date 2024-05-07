#include "wx_utils.h"
#include "pins_config.h"
#include "lora_utils.h"

#include <Wire.h>
#include "bme280_utils.h"
#include "bh1750_utils.h"
#include "wind_rs485_utils.h"
#include "rain_utils.h"


extern uint32_t     lastBeaconTx;
extern int          beaconInterval;
extern bool         beaconUpdate;

extern uint32_t     lastSensorReading;
extern int          sensorReadingInterval;

extern String       Temperature;
extern String       Humidity;
extern String       BarometricPressure;
extern String       Luminosity;
extern String       WindAngle;
extern String       WindDirection;
extern String       WindSpeedMpH;
extern String       WindGust;
extern String       RainLastHr;
extern String       RainLast24Hr;

extern String       firstLine;


//
#define heightCorrection 0       // in meters
extern String       callsign;
extern String       tocall;     // fijo?
extern String       path;
extern String       overlay;    // fijo?
extern String       symbol;     // fijo?
extern String       comment;

String datoGPSLat = "3301.95S";     // reemplazar!
String datoGPSLon = "07134.25W";    // reemplazar!
//


namespace WX_Utils {

    String buildWxStationPacket() {
        //BME280_Utils::readSensor();
        //BH1750_Utils::readSensor();           // "L" si es menor que 1000 W/m2 y "l" si es >= 1000 W/m2 y reemplaza algunos de los campos de lluvia.
        WIND_RS485_Utils::generateData();
        //RAIN_Utils::generateData();

        String wxPacket = WindAngle + "/" + WindSpeedMpH + "g" + WindGust + "t" + Temperature + "r" + RainLastHr + "p" + RainLast24Hr + "L" + Luminosity +"h" + Humidity + "b" + BarometricPressure;
        
        return callsign + ">" + tocall + "," + path + ":=" + datoGPSLat + overlay + datoGPSLon + symbol + wxPacket + comment;
    }

    void loop() {
        RAIN_Utils::loop();

        uint32_t lastWind = millis() - lastSensorReading;
        if (lastWind >= sensorReadingInterval*60*1000) {
        //if (lastWind >= sensorReadingInterval*3*1000) {
            WIND_RS485_Utils::readSensor();
            RAIN_Utils::processMinute();
            lastSensorReading = millis();
        }

        uint32_t lastTx = millis() - lastBeaconTx;
        if (lastTx >= beaconInterval*60*1000) {
        //if (lastTx >= beaconInterval*3*1000) {
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
        Serial.println("\nSensors INI...");
        BME280_Utils::setup();
        BH1750_Utils::setup();
        WIND_RS485_Utils::setup();
        firstLine = callsign;
        Serial.println("\n");
    }
    
}