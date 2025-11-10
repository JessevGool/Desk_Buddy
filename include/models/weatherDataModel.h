#pragma once
#include "apiClient.h"
namespace DeskBuddy
{

    struct DailyWeather {
        String main;
        float temperature;
        float humidity;
    };
    class WeatherDataModel
    {
    public:
        WeatherDataModel() = default;
        void parseWeatherData(const DynamicJsonDocument &doc, size_t dayIndex = 0);
        void parseWeatherDataWeek(const DynamicJsonDocument &doc);
        String location;
        std::vector<DailyWeather> weeklyForecast = {};
    };
} // namespace DeskBuddy
