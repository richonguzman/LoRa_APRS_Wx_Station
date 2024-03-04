#include <TinyGPS++.h>
#include "TimeLib.h"
#include "pins_config.h"
#include "gps_utils.h"

extern HardwareSerial   neo6m_gps;
extern TinyGPSPlus      gps;

namespace GPS_Utils {

    void setup() {
        neo6m_gps.begin(9600, SERIAL_8N1, GPS_TX, GPS_RX);
    }

    void getData() {
        while (neo6m_gps.available() > 0) {
            gps.encode(neo6m_gps.read());
        }
    }

    void setDateFromData() {
        if (gps.time.isValid()) {
            setTime(gps.time.hour(), gps.time.minute(), gps.time.second(), gps.date.day(), gps.date.month(), gps.date.year());
        }
    }

}