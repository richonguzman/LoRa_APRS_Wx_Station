#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include "configuration.h"
#include "boards_pinout.h"
#include "display.h"

extern Configuration    Config;

Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

void displaySetup() {
    Wire.begin(OLED_SDA, OLED_SCL);

    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
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
}

void displayToggle(bool toggle) {
    if (toggle) {
        display.ssd1306_command(SSD1306_DISPLAYON);
    } else {
        display.ssd1306_command(SSD1306_DISPLAYOFF);
    }
}

void displayShow(const String& header, const String& line1, const String& line2, const String& line3, int wait) {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(header);
    display.setCursor(0, 8);
    display.println(line1);
    display.setCursor(0, 16);
    display.println(line2);
    display.setCursor(0, 24);
    display.println(line3);
    display.ssd1306_command(SSD1306_SETCONTRAST);
    display.ssd1306_command(1);
    display.display();
    delay(wait);
}

void displayShow(const String& header, const String& line1, const String& line2, const String& line3, const String& line4, const String& line5, const String& line6, int wait) {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println(header);
    display.setTextSize(1);
    display.setCursor(0, 16);
    display.println(line1);
    display.setCursor(0, 24);
    display.println(line2);
    display.setCursor(0, 32);
    display.println(line3);
    display.setCursor(0, 40);
    display.println(line4);
    display.setCursor(0, 48);
    display.println(line5);
    display.setCursor(0, 56);
    display.println(line6);
    display.ssd1306_command(SSD1306_SETCONTRAST);
    display.ssd1306_command(1);
    display.display();
    delay(wait);
}