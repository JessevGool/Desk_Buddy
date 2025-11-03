#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <uri/UriBraces.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <DHT.h>

#include "displayController.h"

#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
#define WIFI_CHANNEL 6

#define TFT_DC 2
#define TFT_CS 27
#define DHTPIN 5

#define JOYSTICK_H 33
#define JOYSTICK_V 32
#define JOYSTICK_SEL 35

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

DHT dht(DHTPIN, DHT22); // Needs to be a DHT11

DeskBuddy::DisplayController displayController;

void setup()
{
  displayController = DeskBuddy::DisplayController();
  pinMode(JOYSTICK_V, INPUT);
  pinMode(JOYSTICK_H, INPUT);
  pinMode(JOYSTICK_SEL, INPUT_PULLUP);

  dht.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  tft.begin();
  tft.setRotation(1);

  DeskBuddy::DisplayPage mainPage = DeskBuddy::DisplayPage("Main");
  displayController.AddPage(mainPage);
}

void loop()
{
  tft.fillScreen(ILI9341_BLACK);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int vert = analogRead(JOYSTICK_V);
  int horiz = analogRead(JOYSTICK_H);
  bool select = !digitalRead(JOYSTICK_SEL);

  tft.setCursor(0, 0);
  DeskBuddy::DisplayPage currentPage = displayController.GetPage("Main");
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  //tft.println("DHT22");
  tft.print(F("Page:"));
  tft.println(currentPage.GetName().c_str());
  tft.println(F("[....      ]"));
  tft.drawRect(50, 40, 140, 60, ILI9341_WHITE);
  tft.fillRect(51, 41, 100, 58, ILI9341_RED);
  tft.drawCircle(200, 50, 30, ILI9341_GREEN);
  tft.drawCircle(200, 50, 25, ILI9341_RED);
  // tft.print(F("Humidity: "));
  // tft.print(humidity);
  // tft.println(" %");
  // tft.print(F("Vertical Value: "));
  // tft.println(vert);
  // tft.print(F("Horizontal Value: "));
  // tft.println(horiz);
  // tft.print(F("Select Pressed: "));
  // tft.println(select ? "YES" : "NO");
  delay(1000);
}