#include "pins_config.h"
#include "wind_rs485_utils.h"


extern HardwareSerial rs485Serial;

extern String WindDirection;
extern String WindSpeed;
extern String Gust;


namespace WIND_RS485_Utils {
    
    void setup() {
        Serial.println("init : RS485  Module  ...     done!");
        rs485Serial.begin(4800,SERIAL_8N1,RS485_RXD,RS485_TXD); // default speed in bauds
    }

    void readSensor_WindSpeed() {
        WindSpeed            = "002";    // mph
    }

    void readSensor_WindDirection() {
        // aqui comenzamos a calcular
    
        WindDirection        = "123";    // degrees
    }

    void readSensor() {
        readSensor_WindDirection();
        readSensor_WindSpeed();
        //  calcular wind gust!
        Gust                 = "003";    // peak wind speed in last 5 min.
    }

}