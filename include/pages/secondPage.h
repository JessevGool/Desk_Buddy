#pragma once
#include "displayPage.h"

namespace DeskBuddy
{
    class SecondPage : public DisplayPage
    {
    public:
        SecondPage() : DisplayPage("Second") {}
          void draw(Adafruit_ILI9341 &display) override;

          private:
          
    };
}