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
#include "lora_utils.h"
#include "gps_utils.h"
#include "wx_utils.h"
#include "display.h"
#include "utils.h"

HardwareSerial rs485Serial(1);

String firstLine, secondLine, thirdLine, fourthLine, fifthLine, sixthLine, seventhLine;

/*********** TO BE ADDED FROM CONFIGURATION ***********/
String      callsign                = "CA2RXU-13";
String      tocall                  = "APLRW1";
String      path                    = "WIDE1-1";
String      overlay                 = "L";
String      symbol                  = "_";
String      comment                 = "Experimental LoRa Wx Station";

uint8_t     OldSensorAddress        = 0x01;
uint8_t     NewSensorAddress        = 0x02;

float       stationLatitude         = -33.0323050;
float       stationLongitude        = -71.5717292;
/******************************************************/

void setup() {
    Serial.begin(115200);
    delay(4000);
    setup_display();
    show_display(" APRS LoRa", "", "      WX station", "", "       CA2RXU"," ", " ", 4000);

    Utils::pinDeclarations();    
    //Utils::checkWindDireccionSensorAddress();
    //Utils::changeWindDireccionSensorAddress();

    //if (Utils::checkSwitchesStates()) {
    Utils::getI2CAddresses();
    WX_Utils::setupSensors();
    LoRa_Utils::setup();
    GPS_Utils::generateBeacon();
    //}
}

void loop() {
    //if (Utils::checkSwitchesStates()) {
        WX_Utils::loop();
        show_display(firstLine, secondLine, thirdLine, fourthLine, fifthLine, sixthLine, seventhLine, 0);
    //}
}

// TODO******************
// enviar Luminosity con L y l
// mejorar Display