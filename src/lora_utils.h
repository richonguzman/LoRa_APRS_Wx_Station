#ifndef LORA_UTILS_H_
#define LORA_UTILS_H_

#include <Arduino.h>

namespace LoRa_Utils {

    void setup();
    void sendNewPacket(const String& newPacket);
    
}

#endif