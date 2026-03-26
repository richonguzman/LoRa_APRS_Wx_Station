
#include <Wire.h>
#include "configuration.h"
#include "board_pinout.h"
#include "display.h"

#ifdef HAS_DISPLAY
    #include <Adafruit_SSD1306.h>
    #include <Adafruit_GFX.h>
    Adafruit_SSD1306 display(128, 64, &Wire, OLED_RST);
#endif

extern Configuration    Config;

void displaySetup() {
    #ifdef HAS_DISPLAY
        #ifdef OLED_DISPLAY_HAS_RST_PIN
            pinMode(OLED_RST, OUTPUT);
            digitalWrite(OLED_RST, LOW);
            delay(20);
            digitalWrite(OLED_RST, HIGH);
        #endif

        if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
            Serial.println(F("SSD1306 allocation failed"));
            for(;;);
        }
        if (Config.display.turn180) {
            display.setRotation(2);
        }
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.ssd1306_command(SSD1306_SETCONTRAST);
        display.ssd1306_command(1);
        display.display();
        delay(1000);
    #endif
}

void displayToggle(bool toggle) {
    #ifdef HAS_DISPLAY
        if (toggle) {
            display.ssd1306_command(SSD1306_DISPLAYON);
        } else {
            display.ssd1306_command(SSD1306_DISPLAYOFF);
        }
    #endif
}

void displayShow(const String& header, const String& line1, const String& line2, const String& line3, int wait) {
    #ifdef HAS_DISPLAY
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println(header);

        const String* const lines[] = {&line1, &line2, &line3};
        for (int i = 0; i < 3; i++) {
            display.setCursor(0, 8 + (8 * i));
            display.println(*lines[i]);
        }

        display.ssd1306_command(SSD1306_SETCONTRAST);
        display.ssd1306_command(1);
        display.display();
        delay(wait);
    #endif
}

void displayShow(const String& header, const String& line1, const String& line2, const String& line3, const String& line4, const String& line5, const String& line6, int wait) {
    #ifdef HAS_DISPLAY
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(2);
        display.setCursor(0, 0);
        display.println(header);
        display.setTextSize(1);

        const String* const lines[] = {&line1, &line2, &line3, &line4, &line5, &line6};
        for (int i = 0; i < 6; i++) {
            display.setCursor(0, 16 + (8 * i));
            display.println(*lines[i]);
        }
        display.ssd1306_command(SSD1306_SETCONTRAST);
        display.ssd1306_command(1);
        display.display();
        delay(wait);
    #endif
}