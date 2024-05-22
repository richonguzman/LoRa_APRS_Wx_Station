#ifndef PINS_CONFIG_H_
#define PINS_CONFIG_H_

#undef OLED_SDA
#undef OLED_SCL
#undef OLED_RST

#define OLED_SDA                21
#define OLED_SCL                22
#define OLED_RESET              -1

#define LedPin                  25
#define windInfoAddrSwitchPin   13
#define windChangeAddrSwitchPin 14

// LoRA SX1278
#define LORA_SCK                5
#define LORA_MISO               19
#define LORA_MOSI               27
#define LORA_CS                 18
#define LORA_RST                23
#define LORA_IRQ                26

// RS485 to TTL
#define RS485_RXD               34  // UART1 RXD pin
#define RS485_TXD               4   // UART1 TXD pin

// RAIN
#define rainSwitchPin       15

#endif

// ***** Connections ******
/*  BH1750 / GY-302     ESP32/LoRa32 v1.6/v2.1

    VCC                 3.3V
    GND                 GND
    SDA                 io_21
    SCL                 io_22
    ADDR                ---

*/

/*  BME280 uses the same I2C pinouts as BH1750 */

/*  RS485 to TTLWind Direction Sensor

    ESP32       RS485/TTL   |   RS485/TTL   RS485-Wind/DirectionSensor
    3.3V        VCC         |   ---         Brown (to 10-30 VDC external supply) 
    34          RXD         |   A+          Yellow
    4           TXD         |   B-          Blue
    GND         GND         |   GND         Black

*/