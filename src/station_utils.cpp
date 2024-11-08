#include "station_utils.h"
#include "configuration.h"
#include "lora_utils.h"
#include "display.h"
#include "utils.h"
#include <vector>

extern Configuration            Config;
extern uint32_t                 lastRxTime;

uint32_t lastTxTime             = millis();
std::vector<String>             outputPacketBuffer;
std::vector<Packet25SegBuffer>  packet25SegBuffer;


namespace STATION_Utils {

    void clean25SegBuffer() {
        if (!packet25SegBuffer.empty()) {
            if ((millis() - packet25SegBuffer[0].receivedTime) >  25 * 1000) {
                packet25SegBuffer.erase(packet25SegBuffer.begin());
            }
        }
    }

    bool check25SegBuffer(const String& station, const String& textMessage) {
        bool shouldBeIgnored = false;
        if (!packet25SegBuffer.empty()) {
            for (int i = 0; i < packet25SegBuffer.size(); i++) {
                if (packet25SegBuffer[i].station == station && packet25SegBuffer[i].payload == textMessage) {
                    shouldBeIgnored = true;
                }
            }
        }
        if (shouldBeIgnored) {
            return false;
        } else {
            Packet25SegBuffer packet;
            packet.receivedTime = millis();
            packet.station      = station;
            packet.payload      = textMessage;
            packet25SegBuffer.push_back(packet);
            return true;
        }
    }

    void processOutputPacketBuffer() {
        int timeToWait                  = 3 * 1000;      // 3 segs between packet Tx and also Rx ???
        uint32_t lastRx                 = millis() - lastRxTime;
        uint32_t lastTx                 = millis() - lastTxTime;
        if (outputPacketBuffer.size() > 0 && lastTx > timeToWait && lastRx > timeToWait) {
            Serial.println("Sending LoRa APRS Packet ---> " + outputPacketBuffer[0]);
            LoRa_Utils::sendNewPacket(outputPacketBuffer[0]);
            outputPacketBuffer.erase(outputPacketBuffer.begin());
            lastTxTime = millis();
        }
    }

    void addToOutputPacketBuffer(const String& packet) {
        outputPacketBuffer.push_back(packet);
    }

}