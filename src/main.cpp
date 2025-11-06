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

#include "displayController.h"
#include "secrets.h"
#include "progressBar.h"
#include "apiClient.h"

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

constexpr int ADC_CENTER = 2048;       // ESP32 12-bit ADC ~ center
constexpr int DEADZONE = 300;          // ignore small wiggles
constexpr int RIGHT_OFFSET = 800;      // how far from center counts as RIGHT
constexpr unsigned long HOLD_MS = 500; // hold duration to trigger (ms)

static bool rightActive = false;       // currently in RIGHT region
static bool rightFired = false;        // already triggered for this hold
static unsigned long rightStartMs = 0; // when RIGHT started

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

DeskBuddy::DisplayController *displayController = nullptr;
DeskBuddy::ProgressBar *progressBar = nullptr;
DeskBuddy::ApiClient *apiClient = nullptr;
DeskBuddy::DisplayPage *currentPage = nullptr;

bool joystickHoldRight(int horiz)
{
  int delta = horiz - ADC_CENTER;
  bool inRight = (delta >= (DEADZONE + RIGHT_OFFSET));

  if (inRight)
  {
    if (!rightActive)
    {
      rightActive = true;
      rightStartMs = millis();
      rightFired = false;
    }
    else
    {
      if (!rightFired && (millis() - rightStartMs >= HOLD_MS))
      {
        rightFired = true; // single fire per hold
        return true;
      }
    }
  }
  else
  {
    // left/neutral/unknown -> reset
    rightActive = false;
    rightFired = false;
  }
  return false;
}

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
  pinMode(JOYSTICK_V, INPUT);
  pinMode(JOYSTICK_H, INPUT);
  pinMode(JOYSTICK_SEL, INPUT_PULLUP);

  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.println("Connecting to WiFi...");
  // dht.begin();

  // Initialize display controller
  displayController = new DeskBuddy::DisplayController(tft);

  connectToWiFi();

  progressBar = new DeskBuddy::ProgressBar(tft, 60, 150, 200, 30);

  // Get the initial page from displayController (it starts with "Main" page)

  currentPage = &displayController->getPage("Main");

  delay(2000);

  DeskBuddy::ApiClient::Options opt;
  opt.timeout_ms = 7000;
  opt.retries = 2;
  opt.insecure = true;

  apiClient = new DeskBuddy::ApiClient("https://jsonplaceholder.typicode.com", opt);

  {
    DynamicJsonDocument doc(512);
    int code = 0;
    bool ok = apiClient->getJson("/todos/1", doc, &code);
    tft.printf("GET /todos/1 -> %d (%s)\n", code, ok ? "ok" : "fail");
    if (ok)
    {
      tft.printf("title: %s, completed: %s\n",
                 doc["title"].as<const char *>(),
                 (doc["completed"].as<bool>() ? "true" : "false"));
    }
  }
  delay(2000);
  tft.fillScreen(ILI9341_BLACK);
  // Display the current page name
}

void loop()
{

  int vert = analogRead(JOYSTICK_V);
  int horiz = analogRead(JOYSTICK_H);
  int butonState = digitalRead(JOYSTICK_SEL);

  if (joystickHoldRight(horiz))
  {
    DeskBuddy::DisplayPage &next = displayController->getNextPage();
    currentPage = &next; // Update current page pointer
    Serial.printf("Switching page to: %s\n", next.getName().c_str());
    tft.fillScreen(ILI9341_BLACK);
  }
  displayController->drawCurrentPage();
}