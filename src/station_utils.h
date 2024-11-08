#ifndef STATION_UTILS_H_
#define STATION_UTILS_H_


#include <Arduino.h>


struct Packet25SegBuffer {
    uint32_t    receivedTime;
    String      station;
    String      payload;
};

namespace STATION_Utils {

    void clean25SegBuffer();
    bool check25SegBuffer(const String& station, const String& textMessage);
    void processOutputPacketBuffer();
    void addToOutputPacketBuffer(const String& packet);

}

#endif