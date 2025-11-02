#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_DC 2
#define TFT_CS 27
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup() {
  tft.begin();
  tft.setRotation(1);

}

void loop() {

    //tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, 0);
    

    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("DHT SENSOR");
    
    tft.setTextColor(ILI9341_RED);
    tft.setTextSize(2);
    tft.println(" ");
    tft.println(" ");
    tft.print(F("Humidity: "));
    tft.print(F("%"));

    tft.setTextColor(ILI9341_BLUE);
    tft.setTextSize(2);
    tft.println(" ");
    tft.println(" ");
    tft.print(F("Temp: "));
    tft.print((char)223);
    tft.print("C");

    // tft.setTextColor(ILI9341_GREEN);
    // tft.setTextSize(2);
    // tft.println(" ");
    // tft.println(" ");
    // tft.println(" ");
    // tft.println(" ");
    // tft.print("ECE-2024");
    delay(1000);
}