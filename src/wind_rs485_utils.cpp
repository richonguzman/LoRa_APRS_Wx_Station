#include "pins_config.h"
#include "wind_rs485_utils.h"
#include "display.h"


extern HardwareSerial rs485Serial;

extern String WindDirection;
extern String WindSpeed;
extern String Gust;
extern String fifthLine;

extern uint8_t OldSensorAddress;
extern uint8_t NewSensorAddress;
//
extern String anguloViento;
extern String velocidadViento;
//

namespace WIND_RS485_Utils {
    
    void setup() {
        rs485Serial.begin(4800,SERIAL_8N1,RS485_RXD,RS485_TXD); // default speed in bauds
        Serial.println("init : RS485  Module  ...     done!");
    }

    void readSensor_WindSpeed() {
        WindSpeed            = "002";    // mph

    }

    void readSensor_WindDirection() {
        // aqui comenzamos a calcular
    
        WindDirection        = "123";    // degrees
    }

    size_t readN(uint8_t *buf, size_t len) {
        size_t offset = 0, left = len;
        int16_t Tineout = 1500;
        uint8_t *buffer = buf;
        long curr = millis();
        while (left) {
            if (rs485Serial.available()) {
                buffer[offset] = rs485Serial.read();
                offset++;
                left--;
            }
            if (millis() - curr > Tineout) {
                break;
            }
        }
        return offset;
    }

    void addCRC(uint8_t *buf, int len) {
        uint16_t crc = 0xFFFF;
        for (int pos = 0; pos < len; pos++) {
            crc ^= (uint16_t)buf[pos];
            for (int i = 8; i != 0; i--) {
                if ((crc & 0x0001) != 0) {
                    crc >>= 1;
                    crc^= 0xA001;
                } else {
                    crc >>= 1;
                }
            }
        }
        buf[len] = crc % 0x100;           // LSB CRC
        buf[len + 1] = crc / 0x100;       // MSB CRC
    }

    uint16_t CRC16_2(uint8_t *buf, int16_t len) {
        uint16_t crc = 0xFFFF;
        for (int pos = 0; pos < len; pos++) {
            crc ^= (uint16_t)buf[pos];
            for (int i = 8; i != 0; i--) {
                if ((crc & 0x0001) != 0) {
                    crc >>= 1;
                    crc^= 0xA001;
                } else {
                    crc >>= 1;
                }
            }
        }
        crc = ((crc & 0x00ff) << 8) | ((crc & 0xff00) >> 8);
        return crc;
    }

    float readWindSpeed(uint8_t address) {
        uint8_t Data[7] = {0}; //Store the original data packet returned by the sensor
        uint8_t COM[8] = {0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00}; //Command for reading wind speed, adjust for your sensor
        boolean ret = false; //Wind speed acquisition success flag
        float windSpeed = 0;
        long curr = millis();
        long curr1 = curr;
        uint8_t ch = 0;
        COM[0] = address; //Add the complete command package with reference to the communication protocol.
        addCRC(COM , 6); //Add CRC_16 check for reading wind speed commandpacket
        rs485Serial.write(COM, 8); //Send the command of reading the wind speed

        while (!ret) {
            if (millis() - curr > 1000) {
                windSpeed = -1; //If the wind speed has not been read for more than 1000 milliseconds, it will be regarded as a timeout and return -1.
                break;
            }
            if (millis() - curr1 > 100) {
                rs485Serial.write(COM, 8); //If the last command to read the wind speed is sent for more than 100 milliseconds and the return command has not been received, the command to read the wind speed will be re-sent
                curr1 = millis();
            }
            if ((readN(&ch, 1) == 1) && (ch == address)) { //Read and judge the packet header.
                Data[0] = ch;
                if ((readN(&ch, 1) == 1) && (ch == 0x03)) { //Read and judge the packet header.
                    Data[1] = ch;
                    if ((readN(&ch, 1) == 1) && (ch == 0x02)) { //Read and judge the packet header.
                        Data[2] = ch;
                        if ((readN(&Data[3], 4) == 4) && (CRC16_2(Data, 5) == (Data[5] * 256 + Data[6]))) { // Check CRC data packet
                            ret = true;
                            windSpeed = (Data[3] * 256 + Data[4]) / 10.00; // Calculate the wind speed
                        }
                    }
                }
            }
        }
        return windSpeed;
    }

    int readWindDirection(uint8_t address) {
        uint8_t Data[7] = {0}; //Store the original data packet returned by the sensor
        uint8_t COM[8] = {0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00}; // Command for reading wind direction, adjust to your sensor
        boolean ret = false; //Wind direction acquisition success flag
        int windDirection = 0;
        long curr = millis();
        long curr1 = curr;
        uint8_t ch = 0;
        COM[0] = address; //Add the complete command package with reference to the communication protocol.
        addCRC(COM , 6); //Add CRC_16 check for reading wind direction commandpacket
        rs485Serial.write(COM, 8); //Send the command of reading the wind direction
        while (!ret) {
            if (millis() - curr > 1000) {
                windDirection = -1; //If the wind direction has not been read for more than 1000 milliseconds, it will be regarded as a timeout and return -1.
                break;
            }
            if (millis() - curr1 > 100) {
                rs485Serial.write(COM, 8); //If the last command to read the wind directionissent for more than 100 milliseconds and the return command has not beenreceived, the command to read the wind direction will be re-sent
                curr1 = millis();
            }
            if ((readN(&ch, 1) == 1) && (ch == address)) { //Read and judge the packet header.
                Data[0] = ch;
                if ((readN(&ch, 1) == 1) && (ch == 0x03)) { //Read and judge the packet header.
                    Data[1] = ch;
                    if ((readN(&ch, 1) == 1) && (ch == 0x02)) { //Read and judge the packet header.
                        Data[2] = ch;
                        if ((readN(&Data[3], 4) == 4) && (CRC16_2(Data, 5) == (Data[5] * 256 + Data[6]))) { //Checkdata packet
                            ret = true;
                            windDirection = Data[3] * 256 + Data[4]; //Calculatethewind direction
                        }
                    }
                }
            }
        }
        return windDirection;
    }

    void readSensor() {
        //

        //velocidadViento     = String(readWindSpeed(0x01));              // En m/seg
        anguloViento        = String(readWindDirection(0x02));        // (0-7 con separaciones de 45°)
        
        //

        // VER QUE MIERDA CON LA DIRECCION DE LOS SENSORES PARA CAMBIARLOS

        readSensor_WindDirection();
        readSensor_WindSpeed();
        //  calcular wind gust!
        Gust                 = "003";    // peak wind speed in last 5 min.
        //fifthLine = "Wind: " + WindSpeed + "(" + Gust + ")m/s " + WindDirection;
        fifthLine = "Wind: " + WindSpeed + "(" + Gust + ")m/s " + "N/E";
    }

    boolean modifyAddress(uint8_t Address1, uint8_t Address2) {
        Serial.println("iniciando cambio addrress");
        uint8_t ModifyAddressCOM[8] = {0x00, 0x06, 0x07, 0xD0, 0x00, 0x00, 0x00, 0x00};
        boolean ret = false;
        long curr = millis();
        long curr1 = curr;
        uint8_t ch = 0;
        ModifyAddressCOM[0] = Address1;
        ModifyAddressCOM[5] = Address2;
        addCRC(ModifyAddressCOM , 6);
        /*Serial.println(ModifyAddressCOM[0]);
        Serial.println(ModifyAddressCOM[5]);
        Serial.println(ModifyAddressCOM[6]);
        Serial.println(ModifyAddressCOM[7]);*/
        
        rs485Serial.write(ModifyAddressCOM, 8); // sends the register modification request        
        if (Address1 != 0x00) {     // if original address != 0x00, check echoed command is ok = command adressed to 0x00 are not echoed
            while (!ret) {
                if (millis() - curr > 1000) {
                    break;
                }
                if (millis() - curr1 > 100) {
                    rs485Serial.write(ModifyAddressCOM, 8);
                    curr1 = millis();
                }
                if ((readN(&ch, 1) == 1) && (ch == Address1)) {
                    if ((readN(&ch, 1) == 1) && (ch == 0x06)) {
                        if ((readN(&ch, 1) == 1) && (ch == 0x07)) {
                            if ((readN(&ch, 1) == 1) && (ch == 0xD0)) {
                                if ((readN(&ch, 1) == 1) && (ch == 0x00)) {
                                    if ((readN(&ch, 1) == 1) && (ch == Address2)) {
                                        ret = true ;
                                    } else {
                                        ret = false;
                                    }
                                }    
                            }
                        }
                    }
                }
            }
        } else {
            ret = true; // return true if Address1 is 0x00
        } 
        return ret;
    }

    void changeSensorAddress() {
        digitalWrite(LedPin, HIGH);
        if (!modifyAddress(OldSensorAddress,NewSensorAddress)) {
            Serial.println("RS485  No communication with sensor");
            show_display("__RS485__", "", "   NO Comunication", "   with Sensor...", "", "", "", 0);
        } else {
            Serial.println("RS485  Address change OK");
            Serial.println("RS485  Release switch and repower the sensor !");
            show_display("__RS485__","Address changed","","to 0x" + String(NewSensorAddress,HEX));
        }
        for (;;); // loop forever
    }



}