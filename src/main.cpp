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
#include "secrets.h"
#include "progressBar.h"

#define WIFI_SSID_W "Wokwi-GUEST"
#define WIFI_PASSWORD_W ""
#define WIFI_CHANNEL 6

#define TFT_DC 17
#define TFT_CS 5
#define TFT_RST 4
// #define DHTPIN 5

#define JOYSTICK_H 33
#define JOYSTICK_V 32
#define JOYSTICK_SEL 27

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
// DHT dht(DHTPIN, DHT22); // Needs to be a DHT11

DeskBuddy::DisplayController displayController;
DeskBuddy::ProgressBar* progressBar = nullptr;
void setup()
{
  displayController = DeskBuddy::DisplayController();
  pinMode(JOYSTICK_V, INPUT);
  pinMode(JOYSTICK_H, INPUT);
  pinMode(JOYSTICK_SEL, INPUT_PULLUP);

  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.println("Connecting to WiFi...");
  // dht.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    tft.print(".");
  }
  tft.println();
  tft.println("IP address: ");
  tft.println(WiFi.localIP());

  DeskBuddy::DisplayPage mainPage = DeskBuddy::DisplayPage("Main");
  displayController.AddPage(mainPage);

  progressBar = new DeskBuddy::ProgressBar(tft, 60, 150, 200, 30);
  delay(2000);
}

void loop()
{
  tft.fillScreen(ILI9341_BLACK);
  // float humidity = dht.readHumidity();
  // float temperature = dht.readTemperature();
  int vert = analogRead(JOYSTICK_V);
  int horiz = analogRead(JOYSTICK_H);
  int butonState = digitalRead(JOYSTICK_SEL);

  tft.setCursor(0, 0);
  DeskBuddy::DisplayPage currentPage = displayController.GetPage("Main");
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  // tft.println("DHT22");
  tft.print(F("Page:"));
  tft.println(currentPage.GetName().c_str());
  tft.println(F("Joystick Test"));
  tft.setTextSize(1);
  tft.print(F("Vertical Value: "));
  tft.println(vert);
  tft.print(F("Horizontal Value: "));
  tft.println(horiz);
  tft.print(F("Select Pressed: "));
  tft.println(butonState);

  progressBar->setProgress((vert / 4095.0) * 100);
  progressBar->draw();

  // tft.drawCircle(200, 50, 30, ILI9341_GREEN);
  // tft.drawCircle(200, 50, 25, ILI9341_RED);
  delay(1000);
}