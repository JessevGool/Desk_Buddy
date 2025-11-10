#pragma once
#include "apiClient.h"
namespace DeskBuddy
{

    class WeatherDataModel
    {
    public:
        WeatherDataModel() = default;
        void parseWeatherData(const DynamicJsonDocument &doc);
        void parseWeatherDataWeek();
        String location;
        String main;
        float temperature;
        float humidity;
    };
} // namespace DeskBuddy
