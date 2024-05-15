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
void checkWindDireccionSensorAddress() {
    if (digitalRead(AddrInfoPin) == LOW && digitalRead(ChangeAddrPin) == HIGH) {
        delay(3000);
        Serial.println("Starting : RS485 Sensor Address Identifier...");
        WIND_RS485_Utils::setup();
        while(1) {
            WIND_RS485_Utils::checkSensorAddress();
            delay(4000);
        }
    } else {
        Serial.println("Pin14 is LOW, turn it off or disconnect it for Check Addresses");
    }
}

void changeWindDireccionSensorAddress() {
    if (digitalRead(ChangeAddrPin) == LOW && digitalRead(AddrInfoPin) == HIGH) {
        delay(3000);     
        Serial.println("RS485  Sensor address change procedure.");
        WIND_RS485_Utils::setup();
        WIND_RS485_Utils::changeSensorAddress();
    } else {
        Serial.println("Pin13 is LOW, turn it off or disconnect it for Changing Address");
    }
}

bool checkModdifierPins() {
    if (digitalRead(ChangeAddrPin) == HIGH && digitalRead(AddrInfoPin) == HIGH) {
        return true;
    } else {
        return false;
    }
}

void setup() {
    Serial.begin(115200);
    delay(4000);
    setup_display();
    show_display(" APRS LoRa", "", "      WX station", "", "       CA2RXU"," ", 4000);
    pinMode(LedPin, OUTPUT);
    pinMode(rainSwitchPin,INPUT_PULLUP);
    pinMode(AddrInfoPin, INPUT);
    pinMode(ChangeAddrPin, INPUT);
    checkWindDireccionSensorAddress();
    changeWindDireccionSensorAddress();
    if (checkModdifierPins()) {
        WX_Utils::setupSensors();  // esto deberia ir en linea 61 ?
        LoRa_Utils::setup();
    }
}

void loop() {
    if (checkModdifierPins()) {
        WX_Utils::loop();
        show_display(firstLine, secondLine, thirdLine, fourthLine, fifthLine, sixthLine, seventhLine, 0);
    }
}

// TODO******************
// enviar Luminosity con L y l
// mejorar Display