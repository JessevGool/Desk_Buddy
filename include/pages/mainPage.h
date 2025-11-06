#pragma once
#include "displayPage.h"

namespace DeskBuddy
{
    class MainPage : public DisplayPage
    {
    public:
        MainPage() : DisplayPage("Main") {}
        void draw(Adafruit_ILI9341 &display) override;
    };
}