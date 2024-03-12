#include <Arduino.h>
#include "lora_utils.h"
#include "pins_config.h"
#include "wx_utils.h"
#include "display.h"

HardwareSerial rs485Serial(1);

uint32_t    lastBeaconTx    = 0;
bool        beaconUpdate    = true;
int         beaconInterval  = 1; //min

// desde Config
String      callsign        = "CA2RXU-13";
String      tocall          = "APLRW1";
String      path            = "WIDE1-1";
String      overlay         = "L";
String      symbol          = "_";
String      comment         = "Experimental LoRa Wx Station";
//

String Temperature, Humidity, BarometricPressure, Luminosity, WindDirection, WindSpeed, Gust, RainLastHr, RainLast24Hr;
String firstLine, secondLine, thirdLine, fourthLine, fifthLine, sixthLine, seventhLine;

//
String anguloViento = "0";
String velocidadViento = "0";
//

void setup() {
    Serial.begin(115200);
    delay(4000);
    setup_display();
    pinMode(internalLedPin, OUTPUT);
    LoRa_Utils::setup();
    WX_Utils::setupSensors();
}

void loop() { 
    WX_Utils::loop();
    show_display(firstLine, secondLine, thirdLine, fourthLine, fifthLine, sixthLine, seventhLine, 0);
}


// TODO
// enviar Luminosity con L y l
// medir viento 
// grabar 2 addrress
// medir lluvia.
