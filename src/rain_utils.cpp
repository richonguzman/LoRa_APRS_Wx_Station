#include "pins_config.h"
#include <rain_utils.h>

extern String RainLastHr;
extern String RainLast24Hr;
extern String sixthLine;


namespace RAIN_Utils { 

    void setup() {
        Serial.println("sensor lluvia ini");
    }

    void loop() {
        //Serial.println("monitoreando sensor lluvia");
        //lluvia : ultimas 24hrs, ultima hr
        RainLastHr           = "004";    // hundreths of inch
        RainLast24Hr         = "020";    // hundreths of inch
        sixthLine = "R1h: " + RainLastHr + " / R24hr: " + RainLast24Hr;
    }

}