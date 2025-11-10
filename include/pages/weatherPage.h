#pragma once
#include "displayPage.h"
#include "apiClient.h"
#include "secrets.h"
#include "models/weatherDataModel.h"
namespace DeskBuddy
{
    class WeatherPage : public DisplayPage
    {
    public:
        WeatherPage( ApiClient &apiClient,Adafruit_ILI9341 &display);
        void draw() override;
        void handleAction() override;

    private:
        void setup();
        WeatherDataModel _weather;
        static void InfoTaskTrampoline(void *param);
        void InfoTaskLoop();
        ApiClient &_client;

        SemaphoreHandle_t _weatherMutex = nullptr;
        TaskHandle_t _infoTaskHandle = nullptr;
        DynamicJsonDocument _doc{8192};
    };
}