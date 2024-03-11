#ifndef WX_UTILS_H_
#define WX_UTILS_H_

#include <Arduino.h>

namespace WX_Utils {

    void setupBME280();
    void setupBH1750();
    //void readSensor_BME280();
    void loop();
    void setupSensors();

}

#endif