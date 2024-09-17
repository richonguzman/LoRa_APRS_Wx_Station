#ifndef BATTERY_UTILS_H_
#define BATTERY_UTILS_H_

#include <Arduino.h>


namespace BATTERY_Utils {

    float   checkInternalVoltage();
    float   checkExternalVoltage();
    String  generateEncodedTelemetry();

}

#endif