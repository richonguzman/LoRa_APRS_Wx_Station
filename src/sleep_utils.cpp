#include "configuration.h"
#include "sleep_utils.h"
#include "lora_utils.h"
#include "wx_utils.h"


extern  Configuration   Config;
extern  uint32_t        lastWindReading;
extern  uint32_t        lastBeaconTx;

uint32_t                timeToSleep         = 0;


namespace SLEEP_Utils {

    uint32_t getSecondsToSleep() {
        uint32_t currentTime        = millis();
        uint32_t elapsedBeaconTime  = (currentTime - lastBeaconTx) / 1000;
        uint32_t intervalTime;

        if (Config.sensors.windDirectionActive && Config.sensors.windSpeedActive) {
            uint32_t elapsedWindTime = (currentTime - lastWindReading) / 1000;
            intervalTime = 60;  // secs = 1 min
            if (elapsedWindTime < elapsedBeaconTime) {
                return (elapsedWindTime < intervalTime) ? (intervalTime - elapsedWindTime) : 0;
            } else {
                return (elapsedBeaconTime < intervalTime) ? (intervalTime - elapsedBeaconTime) : 0;
            }            
        } else {
            intervalTime = Config.beacon.interval * 60;  // secs
            return (elapsedBeaconTime < intervalTime) ? (intervalTime - elapsedBeaconTime) : 0;
        }
    }

    void start() {
        LoRa_Utils::sleepRadio();
        timeToSleep = getSecondsToSleep();
        esp_sleep_enable_timer_wakeup(timeToSleep * 1000000ULL);   // 1 min = 60sec
        Serial.printf("(Sleeping : %d secs)\n", timeToSleep);
        delay(100);
        esp_light_sleep_start();
    }

}