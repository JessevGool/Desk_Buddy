#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <uri/UriBraces.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <DHT.h>

#include "controllers/displayController.h"
#include "progressBar.h"

#define WIFI_SSID_W "Wokwi-GUEST"
#define WIFI_PASSWORD_W ""
#define WIFI_CHANNEL 6

#define TFT_DC 17
#define TFT_CS 5
#define TFT_RST 4

#define JOYSTICK_H 33
#define JOYSTICK_V 32
#define JOYSTICK_SEL 27

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

DeskBuddy::DisplayController *displayController = nullptr;
DeskBuddy::JoystickController *joystickController = nullptr;
DeskBuddy::ProgressBar *progressBar = nullptr;

void connectToWiFi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(300);
    tft.print(".");
  }
  tft.println();
  tft.printf("\nWiFi OK: %s\n", WiFi.localIP().toString().c_str());
}
void setup()
{
  Serial.begin(115200);

  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.println("Connecting to WiFi...");

  connectToWiFi();

  // Initialize joystick controller
  joystickController = new DeskBuddy::JoystickController(JOYSTICK_H, JOYSTICK_V, JOYSTICK_SEL);

  // Initialize display controller with joystick integration
  displayController = new DeskBuddy::DisplayController(tft, *joystickController);
  DeskBuddy::ApiClient::Options opt;
  opt.insecure = true; // since MCSrvStat has a valid cert, but we can skip validation if needed
  DeskBuddy::ApiClient::init("", opt);
  delay(2000);
  tft.fillScreen(ILI9341_BLACK);

  Serial.println("Setup complete. Starting main loop...");
}

void loop()
{
  // The displayController now handles all input and drawing
  displayController->update();

  // Small delay to prevent excessive CPU usage
  delay(10);
}