#pragma once
#include "displayPage.h"

namespace DeskBuddy
{
    class ClockPage : public DisplayPage
    {
    public:
        ClockPage(Adafruit_ILI9341 &display) : DisplayPage("Clock", display) {}
        void draw() override;
        void handleAction() override;
        void handleUpAction() override {}
        void handleDownAction() override {}
    };
}