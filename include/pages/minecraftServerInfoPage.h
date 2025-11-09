#pragma once
#include "displayPage.h"
#include "apiClient.h"
#include "models/mcServerModel.h"
#include "progressBar.h"
namespace DeskBuddy
{

    class MinecraftServerInfoPage : public DisplayPage
    {
    public:
        MinecraftServerInfoPage(ApiClient &client, Adafruit_ILI9341 &display);
        void draw() override;
        void handleAction() override;

    private:
        ApiClient &_client;
        McServerModel status;
        void setup();
        static void InfoTaskTrampoline(void *param);
        void InfoTaskLoop();

        SemaphoreHandle_t statusMutex = nullptr;
        TaskHandle_t infoTaskHandle = nullptr;
        bool haveData = false;
        DynamicJsonDocument doc{16384};
    };
}