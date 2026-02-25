#include "configuration.h"
#include "sleep_utils.h"
#include "lora_utils.h"
#include "wx_utils.h"


extern  Configuration   Config;
extern  uint32_t        lastWindReading;
extern  uint32_t        lastBeaconTx;

uint32_t                timeToSleep         = 0;


namespace SLEEP_Utils {

    uint32_t timeLeft(uint32_t elapsed, uint32_t interval) {
        return (elapsed < interval) ? (interval - elapsed) : 0;
    }

    uint32_t getSecondsToSleep() {
        uint32_t currentTime            = millis();
        uint32_t elapsedBeaconTime      = (currentTime - lastBeaconTx) / 1000;

        if (Config.sensors.windDirectionActive && Config.sensors.windSpeedActive) {
            uint32_t elapsedWindTime    = (currentTime - lastWindReading) / 1000;
            uint32_t sleepTime = min(timeLeft(elapsedWindTime, 60), timeLeft(elapsedBeaconTime, 60));
            return sleepTime;
        } else {
            uint32_t intervalTime = Config.beacon.interval * 60;
            return timeLeft(elapsedBeaconTime, intervalTime);
        }
    }

    void start() {
        LoRa_Utils::sleepRadio();
        timeToSleep = getSecondsToSleep();
        esp_sleep_enable_timer_wakeup(timeToSleep * 1000000ULL);   // 1 min = 60sec
        Serial.printf("(Sleeping : %d secs)\n", timeToSleep);
        delay(200);
        esp_light_sleep_start();
    }

}