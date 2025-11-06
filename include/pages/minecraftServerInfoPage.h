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
        MinecraftServerInfoPage(ApiClient &client) : DisplayPage("Server Info"), _client(client) {}
        void draw(Adafruit_ILI9341 &display) override;

    private:
        ApiClient &_client;
    };
}