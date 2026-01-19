#pragma once
#include "displayPage.h"

namespace DeskBuddy
{
    class SecondPage : public DisplayPage
    {
    public:
        SecondPage(Adafruit_ILI9341 &display) : DisplayPage("Second", display) {}
        void draw() override;
        void handleAction() override;
                void handleUpAction() override {}
        void handleDownAction() override {}

    private:
        bool displayImage = true;
    };
}