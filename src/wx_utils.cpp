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

//
extern String anguloViento;
extern String velocidadViento;
//


extern String Temperature;
extern String Humidity;
extern String BarometricPressure;
extern String Luminosity;
extern String WindDirection;
extern String WindSpeed;
extern String Gust;
extern String RainLastHr;
extern String RainLast24Hr;

extern String firstLine;


namespace WX_Utils {

    String buildWxStationPacket() {
        BME280_Utils::readSensor();
        BH1750_Utils::readSensor();           // "L" si es menor que 1000 W/m2 y "l" si es >= 1000 W/m2 y reemplaza algunos de los campos de lluvia.
        WIND_RS485_Utils::readSensor();

        String wxPacket = WindDirection + "/" + WindSpeed + "g" + Gust + "t" + Temperature + "r" + RainLastHr + "p" + RainLast24Hr + "L" + Luminosity +"h" + Humidity + "b" + BarometricPressure;
        
        return callsign + ">" + tocall + "," + path + ":=" + datoGPSLat + overlay + datoGPSLon + symbol + wxPacket + comment + " Viento:" + velocidadViento;// + anguloViento
    }

    void loop() {
        RAIN_Utils::loop();
        uint32_t lastTx = millis() - lastBeaconTx;
        //if (lastTx >= beaconInterval*60*1000) {         
        if (lastTx >= beaconInterval*10*1000) {
            beaconUpdate = true;    
        }
        if (beaconUpdate) {            
            String wxPacket = buildWxStationPacket();
            //LoRa_Utils::sendNewPacket(wxPacket);
            Serial.println("Enviando packet ---> " + wxPacket);
            lastBeaconTx = millis();
            beaconUpdate = false;
        }
        /*if (statusAfterBoot) {
            processStatus();
        }*/
    }

    void setupSensors() {
        //Wire.begin();       //Wire.begin(OLED_SDA, OLED_SCL);
        Serial.println("\nSensors INI...");
        BME280_Utils::setup();
        BH1750_Utils::setup();

        WIND_RS485_Utils::setup();

        RAIN_Utils::setup();
        
        firstLine = callsign;
        Serial.println("\n");
    }

}