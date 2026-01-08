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
#include <ArduinoOTA.h>
#include "controllers/displayController.h"
#include "progressBar.h"
#include "secrets.h"


#ifdef SEQ_LOGGER
#include "seqLogger.h"
#endif

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

unsigned long last_ota_time = 0;

void setupNTP()
{
  // Configure NTP
  configTime(3600, 3600, "pool.ntp.org", "time.nist.gov");

  // Wait for time to be set
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println("Time synchronized");
}

void connectToWiFi()
{
  tft.println("Connecting to WiFi...");
  tft.println(local_ip.toString());

  if (!WiFi.config(local_ip, gateway, subnet, dns1, dns2))
  {
    tft.println("STA Failed to configure");
  }

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
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.fillScreen(ILI9341_BLACK);

  connectToWiFi();

  setupNTP();

  ArduinoOTA
      .onStart([]()
               {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else { 
        type = "filesystem";
      }

      Serial.println("Start updating " + type); })
      .onEnd([]()
             { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  {
      if (millis() - last_ota_time > 500) {
        Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
        last_ota_time = millis();
      } })
      .onError([](ota_error_t error)
               {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      } });

  ArduinoOTA.begin();

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

#ifdef SEQ_LOGGER
  DeskBuddy::SeqLogger::GetInstance()->log(
      "Information",
      "ESP32 booted",
      "Main");
#endif
}

void loop()
{
  ArduinoOTA.handle();
  // The displayController now handles all input and drawing
  displayController->update();

  // Small delay to prevent excessive CPU usage
  delay(10);
}