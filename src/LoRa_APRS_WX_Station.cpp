#include <Arduino.h>
#include "lora_utils.h"
#include "pins_config.h"
#include "wx_utils.h"


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

void setup() {
    Serial.begin(115200);
    delay(4000);
    
    pinMode(internalLedPin, OUTPUT);
    LoRa_Utils::setup();
    WX_Utils::setupSensors();
}

void loop() { 
    WX_Utils::loop();
}
// medir luminosidad.
// medir viento.
// medir lluvia.
//---- display?