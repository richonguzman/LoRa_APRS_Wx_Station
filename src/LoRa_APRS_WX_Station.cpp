/*_____________________________________________________________________________________

        ██╗      ██████╗ ██████╗  █████╗      █████╗ ██████╗ ██████╗ ███████╗         
        ██║     ██╔═══██╗██╔══██╗██╔══██╗    ██╔══██╗██╔══██╗██╔══██╗██╔════╝         
        ██║     ██║   ██║██████╔╝███████║    ███████║██████╔╝██████╔╝███████╗         
        ██║     ██║   ██║██╔══██╗██╔══██║    ██╔══██║██╔═══╝ ██╔══██╗╚════██║         
        ███████╗╚██████╔╝██║  ██║██║  ██║    ██║  ██║██║     ██║  ██║███████║         
        ╚══════╝ ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝    ╚═╝  ╚═╝╚═╝     ╚═╝  ╚═╝╚══════╝         
                                                                              
    ██╗    ██╗██╗  ██╗    ███████╗████████╗ █████╗ ████████╗██╗ ██████╗ ███╗   ██╗
    ██║    ██║╚██╗██╔╝    ██╔════╝╚══██╔══╝██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║
    ██║ █╗ ██║ ╚███╔╝     ███████╗   ██║   ███████║   ██║   ██║██║   ██║██╔██╗ ██║
    ██║███╗██║ ██╔██╗     ╚════██║   ██║   ██╔══██║   ██║   ██║██║   ██║██║╚██╗██║
    ╚███╔███╔╝██╔╝ ██╗    ███████║   ██║   ██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║
     ╚══╝╚══╝ ╚═╝  ╚═╝    ╚══════╝   ╚═╝   ╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝

                              Ricardo Guzman - CA2RXU 
                 https://github.com/richonguzman/LoRa_APRS_Tracker
                    (donations : http://paypal.me/richonguzman)
_____________________________________________________________________________________*/

#include <Arduino.h>
#include "wind_rs485_utils.h"
#include "boards_pinout.h"
#include "configuration.h"
#include "lora_utils.h"
#include "gps_utils.h"
#include "wx_utils.h"
#include "display.h"
#include "utils.h"


String          versionDate = "2024.09.20";
Configuration   Config;
HardwareSerial  rs485Serial(1);

String firstLine, secondLine, thirdLine, fourthLine, fifthLine, sixthLine, seventhLine;


void setup() {
    Serial.begin(115200);
    delay(4000);
    displaySetup();
    displayShow(" APRS LoRa", "", "      WX Station", "", ""," ", "  CA2RXU  " + versionDate, 4000);
    Serial.println("\nStarting Weather LoRa APRS Station\n");  

    Utils::pinDeclarations();
    Utils::checkSwitchesStates();

    Utils::getI2CAddresses();
    WX_Utils::setupSensors();
    LoRa_Utils::setup();
    GPS_Utils::generateBeacon();
}

void loop() {
    WX_Utils::loop();
    displayShow(firstLine, secondLine, thirdLine, fourthLine, fifthLine, sixthLine, seventhLine, 0);
}