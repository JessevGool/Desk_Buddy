#include "pages/mainPage.h"

namespace DeskBuddy
{
    void MainPage::draw(Adafruit_ILI9341 &display)
    {
        drawTitle(display);
        display.setTextSize(1);
        display.setCursor(0, 30);
        display.println(F("Main Page Content"));
        display.println(F("Joystick controls:"));
        display.println(F(" - Right: Next page"));
        display.println(F(" - Left: Previous page"));
        display.println(F(" - Select: Action"));
        
        // You can add more content here like sensor readings
        display.setCursor(0, 80);
        display.println(F("Status: Ready"));
    }
}