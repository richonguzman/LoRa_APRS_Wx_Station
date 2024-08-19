#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <Arduino.h>
#include <vector>
#include <FS.h>

class BEACON {
public:
    double  latitude;
    double  longitude;
    String  comment;
    int     interval;
    String  overlay;
    String  symbol;
    String  path;
};


class LoraModule {
public:
    long    txFreq;
    int     spreadingFactor;
    long    signalBandwidth;
    int     codingRate4;
    int     power;
};

class Display {
public:
    bool    alwaysOn;
    int     timeout;
    bool    turn180;
};

class SENSORS {
public:
    bool    rainActive;
    bool    windDirectionActive;
    bool    windSpeedActive;
    bool    bh1750Active;
    bool    bme280Active;
    int     bme280HeightCorrection;
    float   bem280TemperatureCorrection;
};


class Configuration {
public:
    String                  callsign;
    BEACON                  beacon;
    LoraModule              loramodule;
    Display                 display;
    SENSORS                 sensor;
  
    void init();
    void writeFile();
    Configuration();

private:
    bool readFile();
    String _filePath;
};

#endif