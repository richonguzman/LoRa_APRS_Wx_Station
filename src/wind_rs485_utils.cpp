#include "pins_config.h"
#include "wind_rs485_utils.h"


extern HardwareSerial rs485Serial;

extern String WindDirection;
extern String WindSpeed;
extern String Gust;

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

    float readWindSpeed(uint8_t Address) {
        uint8_t Data[7] = {0}; //Store the original data packet returned by the sensor
        uint8_t COM[8] = {0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00}; //Command for reading wind speed, adjust for your sensor
        boolean ret = false; //Wind speed acquisition success flag
        float WindSpeed = 0;
        long curr = millis();
        long curr1 = curr;
        uint8_t ch = 0;
        COM[0] = Address; //Add the complete command package with reference to the communication protocol.
        addCRC(COM , 6); //Add CRC_16 check for reading wind speed commandpacket
        rs485Serial.write(COM, 8); //Send the command of reading the wind speed

        while (!ret) {
            if (millis() - curr > 1000) {
                WindSpeed = -1; //If the wind speed has not been read for more than 1000 milliseconds, it will be regarded as a timeout and return -1.
                break;
            }
            if (millis() - curr1 > 100) {
                rs485Serial.write(COM, 8); //If the last command to read the wind speed is sent for more than 100 milliseconds and the return command has not been received, the command to read the wind speed will be re-sent
                curr1 = millis();
            }
            if ((readN(&ch, 1) == 1) && (ch == Address)) { //Read and judge the packet header.
                Data[0] = ch;
                if ((readN(&ch, 1) == 1) && (ch == 0x03)) { //Read and judge the packet header.
                    Data[1] = ch;
                    if ((readN(&ch, 1) == 1) && (ch == 0x02)) { //Read and judge the packet header.
                        Data[2] = ch;
                        if ((readN(&Data[3], 4) == 4) && (CRC16_2(Data, 5) == (Data[5] * 256 + Data[6]))) { // Check CRC data packet
                            ret = true;
                            WindSpeed = (Data[3] * 256 + Data[4]) / 10.00; // Calculate the wind speed
                        }
                    }
                }
            }
        }
        return WindSpeed;
    }

    /*int readWindDirection(uint8_t Address) {
        uint8_t Data[7] = {0}; //Store the original data packet returned by the sensor
        uint8_t COM[8] = {0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00}; // Command for reading wind direction, adjust to your sensor
        boolean ret = false; //Wind direction acquisition success flag
        int WindDirection = 0;
        long curr = millis();
        long curr1 = curr;
        uint8_t ch = 0;
        COM[0] = Address; //Add the complete command package with reference to the communication protocol.
        addCRC(COM , 6); //Add CRC_16 check for reading wind direction commandpacket
        rs485Serial.write(COM, 8); //Send the command of reading the wind direction
        while (!ret) {
            if (millis() - curr > 1000) {
                WindDirection = -1; //If the wind direction has not been read for more than 1000 milliseconds, it will be regarded as a timeout and return -1.
                break;
            }
            if (millis() - curr1 > 100) {
                rs485Serial.write(COM, 8); //If the last command to read the wind directionissent for more than 100 milliseconds and the return command has not beenreceived, the command to read the wind direction will be re-sent
                curr1 = millis();
            }
            if ((readN(&ch, 1) == 1) && (ch == Address)) { //Read and judge the packet header.
                Data[0] = ch;
                if ((readN(&ch, 1) == 1) && (ch == 0x03)) { //Read and judge the packet header.
                    Data[1] = ch;
                    if ((readN(&ch, 1) == 1) && (ch == 0x02)) { //Read and judge the packet header.
                        Data[2] = ch;
                        if ((readN(&Data[3], 4) == 4) && (CRC16_2(Data, 5) == (Data[5] * 256 + Data[6]))) { //Checkdata packet
                            ret = true;
                            WindDirection = Data[3] * 256 + Data[4]; //Calculatethewind direction
                        }
                    }
                }
            }
        }
        return WindDirection;
    }*/

    void readSensor() {
        //
        //anguloViento        = String(readWindDirection(0x01));        // (0-7 con separaciones de 45°)
        velocidadViento     = String(readWindSpeed(0x01));              // En m/seg
        //

        // VER QUE MIERDA CON LA DIRECCION DE LOS SENSORES PARA CAMBIARLOS

        readSensor_WindDirection();
        readSensor_WindSpeed();
        //  calcular wind gust!
        Gust                 = "003";    // peak wind speed in last 5 min.
    }

}