#ifndef PINS_CONFIG_H_
#define PINS_CONFIG_H_

#undef OLED_SDA
#undef OLED_SCL
#undef OLED_RST


#define OLED_SDA            21
#define OLED_SCL            22
#define OLED_RST            16

#define GPS_RX              12
#define GPS_TX              34
#define BUTTON_PIN          -1
#define LORA_SCK            5
#define LORA_MISO           19
#define LORA_MOSI           27
#define LORA_CS             18  // CS  --> NSS
#define LORA_RST            23
#define LORA_IRQ            26  // IRQ --> DIO0

#endif