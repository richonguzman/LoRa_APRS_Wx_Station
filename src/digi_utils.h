#ifndef DIGI_UTILS_H_
#define DIGI_UTILS_H_

#include <Arduino.h>


namespace DIGI_Utils {

    String  checkForStartingBytes(const String& packet);
    String  buildPacket(const String& path, const String& packet, bool thirdParty, bool crossFreq);
    String  generateDigipeatedPacket(const String& packet, bool thirdParty);
    bool    processReceivedLoRaMessage(const String& sender, const String& packet, bool thirdParty);
    void    processLoRaPacket(const String& packet);

}

#endif