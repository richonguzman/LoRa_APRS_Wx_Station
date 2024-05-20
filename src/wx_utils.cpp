#include <Wire.h>
#include "wind_rs485_utils.h"
#include "boards_pinout.h"
#include "bh1750_utils.h"
#include "bme280_utils.h"
#include "lora_utils.h"
#include "rain_utils.h"
#include "gps_utils.h"
#include "wx_utils.h"


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
extern bool         bme280SensorFound;
extern bool         bh1750SensorFound;
extern String       beaconPacket;

int         sensorReadingInterval   = 1;        // min
uint32_t    lastSensorReading       = 10000;
uint32_t    lastBeaconTx            = 0;
bool        beaconUpdate            = true;     // deberia ser false por que no hay promedio!
int         beaconInterval          = 10;       //min

/*********** TO BE ADDED FROM CONFIGURATION ***********/
extern String       callsign;
extern String       tocall;     // fijo?
extern String       path;
extern String       overlay;    // fijo?
extern String       symbol;     // fijo?
extern String       comment;
/******************************************************/


namespace WX_Utils {

    String buildWxStationPacket() {

        if (bh1750SensorFound) {
            BH1750_Utils::readSensor();           // "L" si es menor que 1000 W/m2 y "l" si es >= 1000 W/m2 y reemplaza algunos de los campos de lluvia.
        } else {
            Luminosity         = "...";
        }

        if (bme280SensorFound) {
            BME280_Utils::readSensor();
        } else {
            Temperature         = "...";
            Humidity            = "..";
            BarometricPressure  = ".....";
        }        
        
        WIND_RS485_Utils::generateData();

        RAIN_Utils::generateData();

        String wxPacket = WindAngle + "/" + WindSpeedMpH + "g" + WindGust + "t" + Temperature + "r" + RainLastHr + "p" + RainLast24Hr + "L" + Luminosity +"h" + Humidity + "b" + BarometricPressure;
        
        return beaconPacket + wxPacket + comment;
    }

    void loop() {
        RAIN_Utils::loop();

        uint32_t lastWind = millis() - lastSensorReading;
        if (lastWind >= sensorReadingInterval * 60 * 1000) {
        //if (lastWind >= sensorReadingInterval * 3 * 1000) {
            WIND_RS485_Utils::readSensor();
            RAIN_Utils::processMinute();
            lastSensorReading = millis();
        }

        uint32_t lastTx = millis() - lastBeaconTx;
        if (lastTx >= beaconInterval * 60 * 1000) {
        //if (lastTx >= beaconInterval * 3 * 1000) {
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