#ifndef UTILS_H_
#define UTILS_H_

#include <Arduino.h>
#include <TimeLib.h>

namespace Utils {

    String createDateString(time_t t);
    String createTimeString(time_t t);

}

#endif