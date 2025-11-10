#include "models/weatherDataModel.h"
namespace DeskBuddy
{

    void WeatherDataModel::parseWeatherData(const DynamicJsonDocument &doc)
    {
        this->location = doc["name"] | "Unknown";
        this->main = doc["weather"][0]["main"] | "Unkn*own";
        this->temperature = doc["main"]["temp"] | 0.0;
        this->humidity = doc["main"]["humidity"] | 0.0;
    }
}

    
