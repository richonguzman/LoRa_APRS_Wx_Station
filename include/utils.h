#ifndef UTILS_H_
#define UTILS_H_

#include <Arduino.h>

namespace Utils {

    void pinDeclarations();
    void checkWindDireccionSensorAddress();
    void changeWindDireccionSensorAddress();
    void checkSwitchesStates();
    void getI2CAddresses();

}

#endif