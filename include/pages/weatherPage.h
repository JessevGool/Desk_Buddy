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
        WeatherPage(ApiClient &apiClient, Adafruit_ILI9341 &display);

        void draw() override;
        void handleAction() override;

    private:
        void setup();

        WeatherDataModel _weatherDay;
        WeatherDataModel _weatherWeek;

        static void InfoTaskTrampoline(void *param);
        void InfoTaskLoop();
        ApiClient &_client;

        SemaphoreHandle_t _weatherMutex = nullptr;
        TaskHandle_t _infoTaskHandle = nullptr;
        DynamicJsonDocument _doc{8192};
        bool _singleDayMode = true;

        uint32_t _dayLastUpdateMs = 0;
        uint32_t _weekLastUpdateMs = 0;

        bool initialDayDataFetched = false;
        bool initialWeekDataFetched = false;
    };
}