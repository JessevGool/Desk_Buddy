#include "pages/weatherPage.h"
#include <PNGdec.h>

PNG png;

namespace DeskBuddy
{
    WeatherPage::WeatherPage(ApiClient &apiClient, Adafruit_ILI9341 &display)
        : DisplayPage("Weather", display), _client(apiClient)
    {
        this->setup();
    }

    void WeatherPage::setup()
    {
        _weatherMutex = xSemaphoreCreateMutex();

        xTaskCreatePinnedToCore(
            InfoTaskTrampoline,
            "Info Task",
            8192,
            this,
            1,
            &_infoTaskHandle,
            1);
    }
    void WeatherPage::InfoTaskTrampoline(void *param)
    {
        auto *self = static_cast<WeatherPage *>(param);
        self->InfoTaskLoop();
        vTaskDelete(nullptr);
    }

    void WeatherPage::InfoTaskLoop()
    {
        for (;;)
        {

            UBaseType_t freeWords = uxTaskGetStackHighWaterMark(nullptr);
            Serial.printf("Weather task free stack: %lu bytes\n", (unsigned long)freeWords * sizeof(StackType_t));

            bool singleDay = _singleDayMode;

            String url;
            int code = 0;
            _doc.clear();

            uint32_t now = millis();
            if (g_httpMutex && xSemaphoreTake(g_httpMutex, portMAX_DELAY) == pdTRUE)
            {
                if (singleDay)
                {
                    // Only fetch if cache is older than 2 minutes

                    if (now - _dayLastUpdateMs >= 120000 || !initialDayDataFetched)
                    {
                        url = String("https://api.openweathermap.org/data/2.5/weather?lat=") +
                              String(LATITUDE, 6) +
                              "&lon=" + String(LONGITUDE, 6) +
                              "&appid=" + String(OPENWEATHER_API_KEY) +
                              "&units=metric";

                        bool ok = _client.getJson(url, _doc, &code);
                        if (ok && _doc.size() > 0)
                        {
                            _weatherDay.parseWeatherData(_doc);

                            if (_weatherMutex &&
                                xSemaphoreTake(_weatherMutex, pdMS_TO_TICKS(50)) == pdTRUE)
                            {
                                _dayLastUpdateMs = now;
                                Serial.println("Day weather updated");
                                xSemaphoreGive(_weatherMutex);
                            }
                        }
                        else
                        {
                            Serial.printf("Day weather fetch failed (ok=%d, code=%d, size=%u)\n",
                                          ok, code, (unsigned)_doc.size());
                        }
                        if (!initialDayDataFetched)
                        {
                            initialDayDataFetched = true;
                        }
                    }
                }
                else
                {
                    if (now - _weekLastUpdateMs >= 120000 || !initialWeekDataFetched)
                    {
                        url = String("https://api.openweathermap.org/data/2.5/forecast?lat=") +
                              String(LATITUDE, 6) +
                              "&lon=" + String(LONGITUDE, 6) +
                              "&appid=" + String(OPENWEATHER_API_KEY) +
                              "&units=metric";

                        bool ok = _client.getJson(url, _doc, &code);
                        if (ok && _doc.size() > 0)
                        {

                            _weatherWeek.parseWeatherDataWeek(_doc);

                            if (_weatherMutex &&
                                xSemaphoreTake(_weatherMutex, pdMS_TO_TICKS(50)) == pdTRUE)
                            {

                                _weekLastUpdateMs = now;
                                Serial.println("Week weather updated");
                                xSemaphoreGive(_weatherMutex);
                            }
                        }
                        else
                        {
                            Serial.printf("Week weather fetch failed (ok=%d, code=%d, size=%u)\n",
                                          ok, code, (unsigned)_doc.size());
                        }
                    }
                }

                xSemaphoreGive(g_httpMutex);
            }

            ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(120000));
        }
    }

    void WeatherPage::draw()
    {
        WeatherDataModel snapshot;

        if (_weatherMutex && xSemaphoreTake(_weatherMutex, pdMS_TO_TICKS(50)) == pdTRUE)
        {
            if (_singleDayMode)
                snapshot = _weatherDay;
            else
                snapshot = _weatherWeek;

            xSemaphoreGive(_weatherMutex);
        }
        else
        {
            drawTitle(display);
            display.setCursor(0, 30);
            display.println("Weather unavailable");
            return;
        }

        drawTitle(display);
        display.setTextSize(1);
        display.setCursor(0, 30);

        if (_singleDayMode)
        {
            if (!snapshot.weeklyForecast.empty())
            {
                display.printf("Weather in %s\nTemperature: %.1f C\nHumidity: %.1f%%",
                               snapshot.location.c_str(),
                               snapshot.weeklyForecast[0].temperature,
                               snapshot.weeklyForecast[0].humidity);
            }
            else
            {
                display.printf("Weather in %s\nLoading...\n", snapshot.location.c_str());
            }
        }
        else
        {
            if (!snapshot.weeklyForecast.empty())
            {
                display.printf("Weekly Forecast for %s\n", snapshot.location.c_str());
                for (size_t i = 0; i < snapshot.weeklyForecast.size(); i++)
                {
                    display.printf("Date %s: %s, %.1f C, %.1f%%, Wind: %.1f m/s, %d degrees\n",
                                   snapshot.weeklyForecast[i].date.c_str(),
                                   snapshot.weeklyForecast[i].main.c_str(),
                                   snapshot.weeklyForecast[i].temperature,
                                   snapshot.weeklyForecast[i].humidity,
                                   snapshot.weeklyForecast[i].windSpeed,
                                   snapshot.weeklyForecast[i].windDirection);
                }
            }
            else
            {

                display.printf("Weekly Forecast for %s\nLoading...\n",
                               snapshot.location.c_str());
            }
        }
    }

    void WeatherPage::handleAction()
    {
        _singleDayMode = !_singleDayMode;

        if (_infoTaskHandle != nullptr)
        {
            xTaskNotifyGive(_infoTaskHandle);
        }

        this->display.fillScreen(ILI9341_BLACK);
        delay(1000);
    }
}