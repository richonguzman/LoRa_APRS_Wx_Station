#include <ArduinoJson.h>
#include <SPIFFS.h>
#include "configuration.h"
#include "display.h"


bool Configuration::readFile() {
    Serial.println("Reading config..");

    File configFile = SPIFFS.open("/wxStation_conf.json", "r");

    if (configFile) {
        StaticJsonDocument<2560> data;

        DeserializationError error = deserializeJson(data, configFile);
        if (error) {
            Serial.println("Failed to read file, using default configuration");
        }

        callsign                            = data["callsign"] | "NOCALL-10";

        beacon.latitude                     = data["beacon"]["latitude"] | 0.0;
        beacon.longitude                    = data["beacon"]["longitude"] | 0.0;
        beacon.comment                      = data["beacon"]["comment"] | "Experimental LoRa APRS Wx Station";
        beacon.interval                     = data["beacon"]["interval"] | 10;
        beacon.overlay                      = data["beacon"]["overlay"] | "L";
        beacon.symbol                       = data["beacon"]["symbol"] | "_";
        beacon.path                         = data["beacon"]["path"] | "WIDE1-1";
        
        loramodule.txFreq                   = data["lora"]["txFreq"] | 433775000;
        loramodule.spreadingFactor          = data["lora"]["spreadingFactor"] | 12;
        loramodule.signalBandwidth          = data["lora"]["signalBandwidth"] | 125000;
        loramodule.codingRate4              = data["lora"]["codingRate4"] | 5;
        loramodule.power                    = data["lora"]["power"] | 20;

        display.alwaysOn                    = data["display"]["alwaysOn"] | true;
        display.timeout                     = data["display"]["timeout"] | 4;
        display.turn180                     = data["display"]["turn180"] | false;

        sensors.bme280Active                = data["sensors"]["bme280Active"] | true;
        sensors.bme280HeightCorrection      = data["sensors"]["bme280HeightCorrection"] | 0;
        sensors.bem280TemperatureCorrection = data["sensors"]["bme280TemperatureCorrection"] | 0.0;
        sensors.bh1750Active                = data["sensors"]["bh1750Active"] | true;
        sensors.windDirectionActive         = data["sensors"]["windDirectionActive"] | true;
        sensors.windSpeedActive             = data["sensors"]["windSpeedActive"] | true;
        sensors.rainActive                  = data["sensors"]["rainActive"] | true;       
        

        configFile.close();
        Serial.println("Config read successfuly");
        return true;
    } else {
        Serial.println("Config file not found");
        return false;
    }
}

Configuration::Configuration() {
    if (!SPIFFS.begin(false)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    } else {
        Serial.println("SPIFFS Mounted");
    }
    readFile();
}