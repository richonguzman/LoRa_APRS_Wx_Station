#ifndef BOARD_PINOUT_H_
#define BOARD_PINOUT_H_

    //  LoRa Radio
    #define HAS_SX1278
    #define RADIO_SCLK_PIN          5
    #define RADIO_MISO_PIN          19
    #define RADIO_MOSI_PIN          27
    #define RADIO_CS_PIN            18
    #define RADIO_RST_PIN           14
    #define RADIO_BUSY_PIN          26
    #define RADIO_WAKEUP_PIN        RADIO_BUSY_PIN
    #define GPIO_WAKEUP_PIN         GPIO_SEL_26

    //  I2C
    #define USE_WIRE_WITH_OLED_PINS

    //  Display
    #define HAS_DISPLAY

    #undef OLED_SDA
    #undef OLED_SCL
    #undef OLED_RST

    // OLED
    #define OLED_SDA                21
    #define OLED_SCL                22
    #define OLED_RST                -1

    //  Aditional Config
    #define INTERNAL_LED_PIN        25      // Green Led
    #define BATTERY_PIN             35

    // RS485 to TTL
    #define RS485_RXD               34  // UART1 RXD pin
    #define RS485_TXD               4   // UART1 TXD pin

    // RAIN
    #define RAIN_SWITCH_PIN         15

    // ***** Connections ******
/*  BH1750 / GY-302     ESP32/LoRa32 v1.6/v2.1

    VCC                 3.3V
    GND                 GND
    SDA                 io_21
    SCL                 io_22
    ADDR                ---

    BME280 uses the same I2C pinouts as BH1750

    RS485 to TTL Wind Direction Sensor

    ESP32       RS485/TTL   |   RS485/TTL   RS485-Wind/DirectionSensor
    3.3V        VCC         |   ---         Brown (to 10-30 VDC external supply)
    34          RXD         |   A+          Yellow
    4           TXD         |   B-          Blue
    GND         GND         |   GND         Black
*/

#endif
