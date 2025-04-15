#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <Arduino.h>
#include <vector>
#include <FS.h>

class Beacon {
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
    bool    bme280Active;
    int     bme280HeightCorrection;
    float   bem280TemperatureCorrection;
    bool    bh1750Active;
    bool    windDirectionActive;
    bool    windSpeedActive;
    bool    rainActive;    
};


class Configuration {
public:
    String                  callsign;
    Beacon                  beacon;
    LoraModule              loramodule;
    Display                 display;
    SENSORS                 sensors;
  
    void init();
    void writeFile();
    Configuration();

private:
    bool readFile();
    String _filePath;
};

#endif