#include "pages/weatherPage.h"



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

            int code = 0;
            _doc.clear();
            const String url = String("https://api.openweathermap.org/data/2.5/weather?lat=") +
                               String(LATITUDE, 6) +
                               "&lon=" + String(LONGITUDE, 6) +
                               "&appid=" + String(OPENWEATHER_API_KEY) +
                               "&units=metric";

            if (g_httpMutex && xSemaphoreTake(g_httpMutex, portMAX_DELAY) == pdTRUE)
            {
                bool ok = _client.getJson(
                    url,
                    _doc, &code);

                if (ok && _doc.size() > 0)
                {
                     WeatherDataModel tmp;
                    tmp.parseWeatherData(_doc);

                     if (_weatherMutex && xSemaphoreTake(_weatherMutex, pdMS_TO_TICKS(50)) == pdTRUE)
                    {
                        Serial.println("Weather data updated");
                        _weather = std::move(tmp);
                      
                        xSemaphoreGive(_weatherMutex);
                    }
                }
                else
                {
                    Serial.printf("Weather fetch failed (ok=%d, code=%d, size=%u)\n",
                                  ok, code, (unsigned)_doc.size());
                }
                xSemaphoreGive(g_httpMutex);
            }
            vTaskDelay(pdMS_TO_TICKS(120000)); // Update every 120 seconds
        }
    }

    void WeatherPage::draw()
    {
        drawTitle(this->display);
        this->display.setTextSize(1);
        this->display.setCursor(0, 30);
        this->display.printf("Weather in %s\nTemperature: %.1f C\nHumidity: %.1f%%",
                             _weather.location.c_str(),
                             _weather.temperature,
                             _weather.humidity);
    }

    void WeatherPage::handleAction()
    {
        // Example action: could refresh weather data
        // For now, just print to serial
        Serial.println("WeatherPage action triggered!");
    }
}