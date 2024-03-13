#include <Arduino.h>
#include "lora_utils.h"
#include "pins_config.h"
#include "wx_utils.h"
#include "display.h"
#include "wind_rs485_utils.h"

HardwareSerial rs485Serial(1);

uint32_t    lastBeaconTx    = 0;
bool        beaconUpdate    = true;
int         beaconInterval  = 1; //min

// desde Config
String      callsign            = "CA2RXU-13";
String      tocall              = "APLRW1";
String      path                = "WIDE1-1";
String      overlay             = "L";
String      symbol              = "_";
String      comment             = "Experimental LoRa Wx Station";

uint8_t     OldSensorAddress    = 0x01;
uint8_t     NewSensorAddress    = 0x02;
//

String Temperature, Humidity, BarometricPressure, Luminosity, WindAngle, WindDirection, WindSpeedMs, WindSpeedKmH, WindSpeedMpH, Gust, RainLastHr, RainLast24Hr;
String firstLine, secondLine, thirdLine, fourthLine, fifthLine, sixthLine, seventhLine;

void setup() {
    Serial.begin(115200);
    delay(4000);
    setup_display();
    show_display(" APRS LoRa", "", "      WX station", "", "       CA2RXU"," ", 4000);
    pinMode(LedPin, OUTPUT);
    WX_Utils::setupSensors();
    pinMode(ChangeAddrPin, INPUT_PULLDOWN);
    if (digitalRead(ChangeAddrPin) == HIGH) {
        Serial.println("RS485  Sensor address change procedure.");
        WIND_RS485_Utils::changeSensorAddress();
    }
    LoRa_Utils::setup();    
}

void loop() { 
    WX_Utils::loop();
    show_display(firstLine, secondLine, thirdLine, fourthLine, fifthLine, sixthLine, seventhLine, 0);
}


// TODO******************

// medir viento REAL

// enviar Luminosity con L y l

// medir lluvia.

// mejorar Display
