#include <Arduino.h>
#include "lora_utils.h"
#include "pins_config.h"
#include "wx_utils.h"
#include "display.h"
#include "wind_rs485_utils.h"

HardwareSerial rs485Serial(1);

uint32_t    lastBeaconTx            = 0;
bool        beaconUpdate            = true;     // deberia ser false por que no hay promedio!
int         beaconInterval          = 10;       //min

int         sensorReadingInterval   = 1;        // min
uint32_t    lastSensorReading       = 10000;
// Wind
int         windArrayIndex          = 0; 
float       windSpeedArray[10]      = {0.0};
int         windDirectionArray[10]  = {0};
// Rain
float       rain60MinArray[60]      = {0.0};
float       rain24HArray[24]        = {0.0};
int         rainTippingCounter      = 0;
float       rainBucketMM            = 0.2794;   // mm watter or 0.3537
int         rain60MinIndex          = -1;
int         rain24HIndex            = 0;
int         rainSwitchState         = HIGH;
int         rainLastSwitchState     = HIGH;
uint32_t    lastDebounceTime        = 0;


String Temperature, Humidity, BarometricPressure, Luminosity, WindAngle, WindDirection, WindSpeedMs, WindSpeedKmH, WindSpeedMpH, WindGust, RainLastHr, RainLast24Hr;
String firstLine, secondLine, thirdLine, fourthLine, fifthLine, sixthLine, seventhLine;

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


void setup() {
    Serial.begin(115200);
    delay(4000);
    setup_display();
    show_display(" APRS LoRa", "", "      WX station", "", "       CA2RXU"," ", 4000);
    pinMode(LedPin, OUTPUT);
    pinMode(rainSwitchPin,INPUT_PULLUP);
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
// enviar Luminosity con L y l
// mejorar Display
