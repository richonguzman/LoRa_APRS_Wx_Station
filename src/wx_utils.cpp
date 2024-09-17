#include <Wire.h>
#include "wind_rs485_utils.h"
#include "battery_utils.h"
#include "boards_pinout.h"
#include "configuration.h"
#include "bh1750_utils.h"
#include "bme280_utils.h"
#include "lora_utils.h"
#include "rain_utils.h"
#include "gps_utils.h"
#include "wx_utils.h"


extern Configuration    Config;
extern String           Temperature;
extern String           Humidity;
extern String           BarometricPressure;
extern String           Luminosity;
extern String           WindAngle;
extern String           WindDirection;
extern String           WindSpeedMpH;
extern String           WindGust;
extern String           RainLastHr;
extern String           RainLast24Hr;
extern String           firstLine;
extern bool             bme280SensorFound;
extern bool             bh1750SensorFound;
extern String           beaconPacket;
extern String           versionDate;

int         sensorReadingInterval   = 1;        // min
uint32_t    lastSensorReading       = 10000;
uint32_t    lastBeaconTx            = 0;
bool        beaconUpdate            = true;     // deberia ser false por que no hay promedio!
bool        statusAfterBoot         = true;
bool        sendStartTelemetry      = true;


namespace WX_Utils {

    String buildDataPacket() {

        if (Config.sensors.bh1750Active && bh1750SensorFound) {
            BH1750_Utils::readSensor();           // "L" si es menor que 1000 W/m2 y "l" si es >= 1000 W/m2 y reemplaza algunos de los campos de lluvia.
        } else {
            Luminosity          = "...";
        }

        if (Config.sensors.bme280Active && bme280SensorFound) {
            BME280_Utils::readSensor();
        } else {
            Temperature         = "...";
            Humidity            = "..";
            BarometricPressure  = ".....";
        }        
        
        if (Config.sensors.windDirectionActive || Config.sensors.windSpeedActive) {
            WIND_RS485_Utils::generateData();
        } else {
            WindAngle           = "...";
            WindSpeedMpH        = "...";
            WindGust            = "...";
        }

        if (Config.sensors.rainActive) {
            RAIN_Utils::generateData();
        } else {
            RainLastHr          = "...";
            RainLast24Hr        = "...";
        }

        String wxPacket = beaconPacket;
        
        wxPacket += WindAngle;
        wxPacket += "/";
        wxPacket += WindSpeedMpH;
        
        if (Config.sensors.windDirectionActive || Config.sensors.windSpeedActive) {    
            wxPacket += "g";
            wxPacket += WindGust;
        }

        wxPacket += "t";
        wxPacket += Temperature;
        wxPacket += "h";
        wxPacket += Humidity;
        wxPacket += "b";
        wxPacket += BarometricPressure;

        if (Config.sensors.rainActive) {
            wxPacket += "r";
            wxPacket += RainLastHr;
            wxPacket += "p";
            wxPacket += RainLast24Hr;
        }

        if (Config.sensors.bh1750Active && bh1750SensorFound) {
            wxPacket += "L";
            wxPacket += Luminosity;
        }
        
        if (Config.battery.sendInternalVoltage || Config.battery.sendExternalVoltage) {
            if (Config.battery.sendAsEncodedTelemetry) {
                return wxPacket + Config.beacon.comment;
            } else {
                return wxPacket + Config.beacon.comment + "BAT + ExtV";
            }
        } else {
            return wxPacket + Config.beacon.comment;
        }        
    }

    void processStatus() {
        delay(4000);
        String status = Config.callsign;
        status += ">APLRW1";
        if (Config.beacon.path != "") {
            status += ",";
            status += Config.beacon.path;
        }
        status += ":>https://github.com/richonguzman/LoRa_APRS_Wx_Station ";
        status += versionDate;
        LoRa_Utils::sendNewPacket(status);
        statusAfterBoot = false;
    }
    
    void loop() {
        if (Config.sensors.rainActive) RAIN_Utils::loop();

        uint32_t lastWind = millis() - lastSensorReading;
        if (lastWind >= sensorReadingInterval * 60 * 1000) {
            if (Config.sensors.windDirectionActive || Config.sensors.windSpeedActive) WIND_RS485_Utils::readSensor();
            if (Config.sensors.rainActive) RAIN_Utils::processMinute();
            lastSensorReading = millis();
        }

        uint32_t lastTx = millis() - lastBeaconTx;
        if (lastTx >= Config.beacon.interval * 60 * 1000) {
            beaconUpdate = true;
        }
        if (beaconUpdate) {
            String wxPacket = buildDataPacket();

            #if defined(BATTERY_PIN) || defined(EXT_VOLT_PIN)
                if (sendStartTelemetry && Config.battery.sendAsEncodedTelemetry) {                
                    String sender = Config.callsign;
                    for (int i = sender.length(); i < 9; i++) {
                        sender += ' ';
                    }
                    String basePacket = Config.callsign + ">APLRG1,WIDE1-1::" + sender + ":";
                    String tempPacket = basePacket + "EQNS.";

                    if (Config.battery.sendInternalVoltage) {
                        tempPacket += "0,0.01,0";
                    }
                    if (Config.battery.sendExternalVoltage) {
                        tempPacket += String(Config.battery.sendInternalVoltage ? "," : "") + "0,0.02,0";
                    }
                    LoRa_Utils::sendNewPacket(tempPacket);
                    delay(3000);

                    tempPacket = basePacket + "UNIT.";
                    if (Config.battery.sendInternalVoltage) {
                        tempPacket += "VDC";
                    }
                    if (Config.battery.sendExternalVoltage) {
                        tempPacket += String(Config.battery.sendInternalVoltage ? "," : "") + "VDC";
                    }
                    LoRa_Utils::sendNewPacket(tempPacket);
                    delay(3000);

                    tempPacket = basePacket + "PARM.";
                    if (Config.battery.sendInternalVoltage) {
                        tempPacket += "V_Batt";
                    }
                    if (Config.battery.sendExternalVoltage) {
                        tempPacket += String(Config.battery.sendInternalVoltage ? "," : "") + "V_Ext";
                    }
                    LoRa_Utils::sendNewPacket(tempPacket);
                    delay(3000);
                    sendStartTelemetry = false;
                }            

                if (Config.battery.sendAsEncodedTelemetry) {
                    wxPacket += BATTERY_Utils::generateEncodedTelemetry();
                }
            #endif

            Serial.println("Sending LoRa APRS Packet ---> " + wxPacket);
            LoRa_Utils::sendNewPacket(wxPacket);
            lastBeaconTx = millis();
            beaconUpdate = false;
        }
        if (statusAfterBoot) {
            processStatus();
        }
    }

    void setupSensors() {
        Serial.println("Sensors INI...");
        if (Config.sensors.bme280Active) BME280_Utils::setup();
        if (Config.sensors.bh1750Active) BH1750_Utils::setup();
        if (Config.sensors.windDirectionActive || Config.sensors.windSpeedActive) WIND_RS485_Utils::setup();
        firstLine = Config.callsign;
    }
    
}