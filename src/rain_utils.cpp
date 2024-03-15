#include "pins_config.h"
#include <rain_utils.h>

extern String   RainLastHr;
extern String   RainLast24Hr;
extern String   sixthLine;

extern float    rain60MinArray[];
extern float    rain24HArray[];
extern float    rainMinute;
extern float    rainBucketMM;
extern int      rain60MinIndex;
extern int      rain24HIndex;




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
    }

    void processHour() {
        float rain1H = 0.0;
        for (int i = 0; i < 60; i++) {
            rain1H += rain60MinArray[i];
        }
        rain24HArray[rain24HIndex] = rain1H;
        rain24HIndex = (rain24HIndex + 1) % 24;
    }

    void processMinute() {
        if (rain60MinIndex > -1) {
            rain60MinArray[rain60MinIndex] = rainMinute;
        }
        if (rain60MinIndex == 59) {
            processHour();            
        }
        rain60MinIndex = (rain60MinIndex + 1) % 60;
        rainMinute = 0;
    }


    void loop() {
        bool switchLluvia = true;
        if (switchLluvia) {                 // si se activa el sensor de lluvia // revisar DEBOUNCE???
            rainMinute += rainBucketMM;
        }

        /*if ((bucketPositionA==false)&&(digitalRead(RainPin)==HIGH)){
            bucketPositionA=true;
            dailyRain+=bucketAmount;                               // update the daily rain
        }
        if ((bucketPositionA==true)&&(digitalRead(RainPin)==LOW)){
            bucketPositionA=false;  
        }*/


        sixthLine = "R1h: " + RainLastHr + " / R24hr: " + RainLast24Hr;
    }

}