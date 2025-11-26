#include "models/weatherDataModel.h"
#include <string>

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
        day.windDirection = doc["wind"]["deg"] | 0;
        day.windSpeed = doc["wind"]["speed"] | 0.0;
    }

    void WeatherDataModel::parseWeatherDataWeek(const DynamicJsonDocument &doc)
    {
        location = doc["city"]["name"] | doc["name"] | "Unknown";
        size_t listSize = doc["list"].size();

        // Make sure the vector is large enough
        if (weeklyForecast.size() < 5)
        {
            weeklyForecast.resize(5);
        }
        String lastDate = "";
        size_t dayCount = 0;
        for (size_t i = 0; i < listSize; i++)
        {
            String dtxt = doc["list"][i]["dt_txt"] | "";
            int splitter = dtxt.indexOf(' ');
            String date = dtxt.substring(0, splitter);
            String time = dtxt.substring(splitter + 1);

            // Only add the first entry so we have info for the first day and only add 12:00 entries for subsequent days
            if (date != lastDate && (lastDate == "" || time == "12:00:00"))
            {
                if (dayCount >= 5)
                    break;
                lastDate = date;
                DailyWeather &day = weeklyForecast[dayCount];
                day.main = doc["list"][i]["weather"][0]["main"] | "Unknown";
                day.date = date;
                day.temperature = doc["list"][i]["main"]["temp"] | 0.0;
                day.humidity = doc["list"][i]["main"]["humidity"] | 0.0;
                day.windDirection = doc["list"][i]["wind"]["deg"] | 0;
                day.windSpeed = doc["list"][i]["wind"]["speed"] | 0.0;
                dayCount++;
            }
        }
    }
}
