#pragma once
#include "displayPage.h"

namespace DeskBuddy
{
    class MainPage : public DisplayPage
    {
    public:
        MainPage(Adafruit_ILI9341 &display) : DisplayPage("Main", display) {}
        void draw() override;
        void handleAction() override;
    };
}