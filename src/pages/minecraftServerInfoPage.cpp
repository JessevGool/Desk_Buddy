#include "pages/minecraftServerInfoPage.h"



namespace DeskBuddy
{

    MinecraftServerInfoPage::MinecraftServerInfoPage(ApiClient &client, Adafruit_ILI9341 &display)
        : DisplayPage("Server Info", display), _client(client)
    {
        this->setup();
    }

    void MinecraftServerInfoPage::setup()
    {
        statusMutex = xSemaphoreCreateMutex();
        xTaskCreatePinnedToCore(
            InfoTaskTrampoline,
            "Info Task",
            8192,
            this,
            1,
            &infoTaskHandle,
            1);
    }

    void MinecraftServerInfoPage::InfoTaskTrampoline(void *param)
    {
        auto *self = static_cast<MinecraftServerInfoPage *>(param);
        self->InfoTaskLoop();
        vTaskDelete(nullptr);
    }
    void MinecraftServerInfoPage::InfoTaskLoop()
    {
        for (;;)
        {
            int code = 0;
            doc.clear();
            if (g_httpMutex && xSemaphoreTake(g_httpMutex, portMAX_DELAY) == pdTRUE)
            {
                bool ok = _client.getJson(
                    "https://api.mcsrvstat.us/3/play.cubecraft.net",
                    doc, &code);

                if (ok && doc.size() > 0)
                {
                    McServerModel tmp;
                    tmp.parseMcServerStatus(doc);

                    if (statusMutex && xSemaphoreTake(statusMutex, pdMS_TO_TICKS(50)) == pdTRUE)
                    {
                        Serial.println("MC status updated");
                        status = std::move(tmp);
                        haveData = status.isValid();
                        xSemaphoreGive(statusMutex);
                    }
                }
                else
                {
                    Serial.printf("MC fetch failed (ok=%d, code=%d, size=%u)\n",
                                  ok, code, (unsigned)doc.size());
                }
                xSemaphoreGive(g_httpMutex);
            }

            vTaskDelay(pdMS_TO_TICKS(5000));
        }
    }

    void MinecraftServerInfoPage::draw()
    {
        drawTitle(this->display);
        this->display.setTextSize(1);
        this->display.setCursor(0, 30);
        this->display.println(F("Minecraft Server Info"));

        McServerModel snap;
        bool snapValid = false;

        if (statusMutex && xSemaphoreTake(statusMutex, pdMS_TO_TICKS(10)) == pdTRUE)
        {
            snap = status;
            snapValid = haveData;
            xSemaphoreGive(statusMutex);
        }

        if (!snapValid)
        {
            display.println(F("Loading..."));
            display.println(F("(If this stays, check WiFi/API)"));
            return;
        }

        display.printf("IP: %s\n", snap.ip.c_str());
        display.printf("Port: %u\n", snap.port);
        display.printf("Online: %s\n", snap.online ? "Yes" : "No");

        int maxPlayers = this->status.playersMax > 0 ? this->status.playersMax : 1;
        int playerPercentage = (this->status.playersOnline * 100) / maxPlayers;
        display.printf("Players: %d/%d\n", snap.playersOnline, snap.playersMax);

        // Draw Progress Bar on next line based on Cursor Y
        int textHeight = 8 * 1;
        ProgressBar pb(display, display.getCursorX(), display.getCursorY(), 4, textHeight, true);
        pb.setProgress(playerPercentage);
        pb.draw();
    }

    void MinecraftServerInfoPage::handleAction()
    {
        // No action defined for this page yet
    }
}
