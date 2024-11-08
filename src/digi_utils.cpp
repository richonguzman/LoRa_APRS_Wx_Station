#include <WiFi.h>
#include "configuration.h"
#include "station_utils.h"
#include "query_utils.h"
#include "digi_utils.h"
#include "utils.h"

extern Configuration    Config;


namespace DIGI_Utils {

    String checkForStartingBytes(const String& packet) {
        if (packet.indexOf("\x3c\xff\x01") != -1) {
            return packet.substring(0, packet.indexOf("\x3c\xff\x01"));
        } else {
            return packet;
        }
    }

    String buildPacket(const String& path, const String& packet, bool thirdParty) {
        String packetToRepeat = packet.substring(0, packet.indexOf(",") + 1);
        String tempPath = path;

        if (path.indexOf("WIDE1-1") != -1 && (Config.digi.mode == 2 || Config.digi.mode == 3)) {
            tempPath.replace("WIDE1-1", Config.callsign + "*");
        } else if (path.indexOf("WIDE2-") != -1 && Config.digi.mode == 3) {
            if (path.indexOf(",WIDE1*") != -1) {
                tempPath.remove(path.indexOf(",WIDE1*"), 7);
            }
            if (path.indexOf("*") != -1) {
                tempPath.remove(path.indexOf("*"), 1);
            }
            if (path.indexOf("WIDE2-1") != -1) {
                tempPath.replace("WIDE2-1", Config.callsign + "*");
            } else if (path.indexOf("WIDE2-2") != -1) {
                tempPath.replace("WIDE2-2", Config.callsign + "*,WIDE2-1");
            } else {
                return "";
            }
        }
        packetToRepeat += tempPath;
        if (thirdParty) {
            packetToRepeat += checkForStartingBytes(packet.substring(packet.indexOf(":}")));
        } else {
            packetToRepeat += checkForStartingBytes(packet.substring(packet.indexOf(":")));
        }
        return packetToRepeat;
    }

    String generateDigipeatedPacket(const String& packet, bool thirdParty){
        String temp;
        if (thirdParty) { // only header is used
            const String& header = packet.substring(0, packet.indexOf(":}"));
            temp = header.substring(header.indexOf(">") + 1);
        } else {
            temp = packet.substring(packet.indexOf(">") + 1, packet.indexOf(":"));
        }
        if (temp.indexOf(",") > 2) { // checks for path
            const String& path = temp.substring(temp.indexOf(",") + 1); // after tocall
            if (Config.digi.mode == 2) {
                if (path.indexOf("WIDE1-1") != - 1) {
                    return buildPacket(path, packet, thirdParty);
                } else {
                    return "";
                }
            } else if (Config.digi.mode == 3) {
                if (path.indexOf("WIDE1-1") != -1 || path.indexOf("WIDE2-") != -1) {
                    int wide1Index = path.indexOf("WIDE1-1");
                    int wide2Index = path.indexOf("WIDE2-");

                    // WIDE1-1 && WIDE2-n   /   only WIDE1-1    /   only WIDE2-n
                    if ((wide1Index != -1 && wide2Index != -1 && wide1Index < wide2Index) || (wide1Index != -1 && wide2Index == -1) || (wide1Index == -1 && wide2Index != -1)) {
                        return buildPacket(path, packet, thirdParty);
                    }
                    return "";
                } else {
                    return "";
                }
            } else {
                return "";
            }
        } else if (temp.indexOf(",") == -1 && (Config.digi.mode == 2 || Config.digi.mode == 3)) {
            return buildPacket("", packet, thirdParty);
        } else {
            return "";
        }
    }

    bool processReceivedLoRaMessage(const String& sender, const String& packet, bool thirdParty) {
        String receivedMessage;
        if (packet.indexOf("{") > 0) {     // ack?
            String ackMessage = "ack";
            ackMessage.concat(packet.substring(packet.indexOf("{") + 1));
            ackMessage.trim();
            //Serial.println(ackMessage);
            
            String addToBuffer = Config.callsign;
            addToBuffer += ">APLRG1";
            if (!thirdParty) addToBuffer += ",RFONLY";
            if (Config.beacon.path != "") {
                addToBuffer += ",";
                addToBuffer += Config.beacon.path;
            }
            addToBuffer += "::";

            String processedSender = sender;
            for (int i = sender.length(); i < 9; i++) {
                processedSender += ' ';
            }
            addToBuffer += processedSender;

            addToBuffer += ":";
            addToBuffer += ackMessage;
            STATION_Utils::addToOutputPacketBuffer(addToBuffer);
            receivedMessage = packet.substring(packet.indexOf(":") + 1, packet.indexOf("{"));
        } else {
            receivedMessage = packet.substring(packet.indexOf(":") + 1);
        }
        if (receivedMessage.indexOf("?") == 0) {
            STATION_Utils::addToOutputPacketBuffer(QUERY_Utils::process(receivedMessage, sender, thirdParty));
            return true;
        } else {
            return false;
        }
    }

    void processLoRaPacket(const String& packet) {        
        if (packet != "") {
            if ((packet.substring(0, 3) == "\x3c\xff\x01") && (packet.indexOf("NOGATE") == -1)) {
                bool thirdPartyPacket = false;
                String temp, Sender;
                int firstColonIndex = packet.indexOf(":");
                if (firstColonIndex > 5 && firstColonIndex < (packet.length() - 1) && packet[firstColonIndex + 1] == '}' && packet.indexOf("TCPIP") > 0) {   // 3rd Party 
                    thirdPartyPacket = true;
                    temp    = packet.substring(packet.indexOf(":}") + 2);
                    Sender  = temp.substring(0, temp.indexOf(">"));
                } else {
                    temp    = packet.substring(3);
                    Sender  = packet.substring(3, packet.indexOf(">"));
                }
                if (Sender != Config.callsign) {        // Avoid listening to own packets
                    if (!thirdPartyPacket && !Utils::checkValidCallsign(Sender)) {
                        return;
                    }
                    if (STATION_Utils::check25SegBuffer(Sender, temp.substring(temp.indexOf(":") + 2))) {
                        bool queryMessage = false;
                        if (temp.indexOf("::") > 10) {   // it's a message
                            String AddresseeAndMessage  = temp.substring(temp.indexOf("::") + 2);
                            String Addressee            = AddresseeAndMessage.substring(0, AddresseeAndMessage.indexOf(":"));
                            Addressee.trim();
                            if (Addressee == Config.callsign) {     // it's a message for me!
                                queryMessage = processReceivedLoRaMessage(Sender, AddresseeAndMessage, thirdPartyPacket);
                            }
                        }
                        if (!queryMessage) {
                            String loraPacket = generateDigipeatedPacket(packet.substring(3), thirdPartyPacket);
                            if (loraPacket != "") {
                                STATION_Utils::addToOutputPacketBuffer(loraPacket);
                            }
                        }
                    }
                }
            }
        }
    }

}