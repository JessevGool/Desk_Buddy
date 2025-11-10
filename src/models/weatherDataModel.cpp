#include "models/weatherDataModel.h"
namespace DeskBuddy
{

    void WeatherDataModel::parseWeatherData(const DynamicJsonDocument &doc, size_t dayIndex)
    {
        location = doc["name"] | "Unknown";

        // Make sure the vector is large enough, but don't clear old data
        if (weeklyForecast.size() <= dayIndex)
        {
            weeklyForecast.resize(dayIndex + 1);
        }

        DailyWeather &day = weeklyForecast[dayIndex];
        day.main = doc["weather"][0]["main"] | "Unknown";
        day.temperature = doc["main"]["temp"] | 0.0;
        day.humidity = doc["main"]["humidity"] | 0.0;
    }

    void WeatherDataModel::parseWeatherDataWeek(const DynamicJsonDocument &doc)
    {;
        location = doc["city"]["name"] | doc["name"] | "Unknown";
        Serial.printf("Raw Response: %s\n", doc.as<String>().c_str());
        size_t listSize = doc["list"].size();

        if (weeklyForecast.size() < listSize)
        {
            weeklyForecast.resize(listSize);
        }

        for (size_t i = 0; i < listSize; i++)
        {
            DailyWeather &day = weeklyForecast[i];
            day.main = doc["list"][i]["weather"][0]["main"] | "Unknown";

            day.temperature = doc["list"][i]["main"]["temp"] | 0.0;
            day.humidity = doc["list"][i]["main"]["humidity"] | 0.0;
        }
    }
}
