#ifndef BOARD_PINOUT_H_
#define BOARD_PINOUT_H_

    //  LoRa Radio
    #define HAS_SX1262
    #define HAS_TCXO
    #define RADIO_SCLK_PIN          9
    #define RADIO_MISO_PIN          11
    #define RADIO_MOSI_PIN          10
    #define RADIO_CS_PIN            8
    #define RADIO_RST_PIN           12
    #define RADIO_DIO1_PIN          14
    #define RADIO_BUSY_PIN          13
    #define RADIO_WAKEUP_PIN        RADIO_DIO1_PIN
    #define GPIO_WAKEUP_PIN         GPIO_SEL_14

    //  I2C
    #define SENSOR_I2C_BUS          Wire1
    #define BOARD_I2C_SDA           41
    #define BOARD_I2C_SCL           42

    //  Aditional Config
    #define INTERNAL_LED_PIN        35
    #define BATTERY_PIN             1

    #define ADC_CTRL_PIN            37
    #define ADC_CTRL_ON_STATE       LOW
    #define VEXT_CTRL_PIN           36
    #define VEXT_CTRL_ON_STATE      LOW

    // RS485 to TTL
    #define RS485_RXD               5   // UART1 RXD pin
    #define RS485_TXD               4   // UART1 TXD pin

    // RAIN
    #define RAIN_SWITCH_PIN         6

    // ***** Connections ******
/*  BH1750 / GY-302     ESP32S3

    VCC                 3.3V
    GND                 GND
    SDA                 41
    SCL                 42
    ADDR                ---

    BME280 uses the same I2C pinouts as BH1750

    RS485 to TTL Wind Direction Sensor

    ESP32       RS485/TTL   |   RS485/TTL   RS485-Wind/DirectionSensor
    3.3V        VCC         |   ---         Brown (to 10-30 VDC external supply)
    5           RXD         |   A+          Yellow
    4           TXD         |   B-          Blue
    GND         GND         |   GND         Black
*/

#endif