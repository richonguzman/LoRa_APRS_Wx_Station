#include "pins_config.h"
#include <rain_utils.h>

extern String   RainLastHr;
extern String   RainLast24Hr;
extern String   sixthLine;

extern float    rain60MinArray[];
extern float    rain24HArray[];
extern int      rainTippingCounter;
extern float    rainBucketMM;
extern int      rain60MinIndex;
extern int      rain24HIndex;
extern int      rainSwitchState;
extern int      rainLastSwitchState;
extern uint32_t lastDebounceTime;

int     debounceDelay   = 50;


namespace RAIN_Utils { 

    String generateRain1HString() {           // last 1 Hr
        float rain1H = 0;
        for (int i = 0; i < 60; i++) rain1H += rain60MinArray[i];
        String rain1HStr = String((int)(rain1H * 3.93701));  // mmToHundredthsInch
        switch (rain1HStr.length()) {
            case 1:
                return "00" + rain1HStr;
                break;
            case 2:
                return "0" + rain1HStr;
                break;
            case 3:
                return rain1HStr;
                break;
            default:
                return "000";
                break;
        }
    }

    String generateRain24HString() {          // last 24 Hr
        float rain24H = 0;
        for (int i = 0; i < 24; i++) rain24H += rain24HArray[i];
        String rain24HStr = String((int)(rain24H * 3.93701));  // mmToHundredthsInch
        switch (rain24HStr.length()) {
            case 1:
                return "00" + rain24HStr;
                break;
            case 2:
                return "0" + rain24HStr;
                break;
            case 3:
                return rain24HStr;
                break;
            default:
                return "000";
                break;
        }
    }

    void generateData() {
        RainLastHr = generateRain1HString();
        RainLast24Hr = generateRain24HString();
        sixthLine = "R1h: " + RainLastHr + " / R24hr: " + RainLast24Hr;
    }

    void processMinute() {
        if (rain60MinIndex > -1) {
            rain60MinArray[rain60MinIndex] = rainTippingCounter * rainBucketMM;
            rain24HArray[rain24HIndex] += rainTippingCounter * rainBucketMM;
        }
        if (rain60MinIndex == 59) {
            rain24HIndex = (rain24HIndex + 1) % 24;
            rain24HArray[rain24HIndex] = 0.0;
        }
        rain60MinIndex = (rain60MinIndex + 1) % 60;
        rainTippingCounter = 0;
    }

    void loop() {
        int rainSwitchReading = digitalRead(rainSwitchPin);
        if (rainSwitchReading != rainLastSwitchState) {         // Check if the button state has changed
            lastDebounceTime = millis();                        // Reset the debounce timer
        }
        if ((millis() - lastDebounceTime) > debounceDelay) {    // Check if the debounce delay has elapsed
            if (rainSwitchReading != rainSwitchState) {         // Update the button state only if the button state has been stable for the debounce delay
                rainSwitchState = rainSwitchReading;
                if (rainSwitchState == LOW) { 
                    rainTippingCounter++;
                }
            }
        }
        rainLastSwitchState = rainSwitchReading;                // Save the current button state for comparison in the next iteration
    }

}