#ifndef WIND_RS485_UTILS_H_
#define WIND_RS485_UTILS_H_

#include <Arduino.h>

namespace WIND_RS485_Utils {
    
    void setup();
    void readSensor_WindSpeed();
    void readSensor_WindDirection();
    void readSensor();
    void generateData();

    void checkSensorAddress();

    boolean modifySensorAddress(uint8_t Address1, uint8_t Address2);
    void changeSensorAddress();

}

#endif