#include "pages/mainPage.h"
namespace DeskBuddy
{
    void MainPage::draw(Adafruit_ILI9341 &display)
    {
        drawTitle(display);
        display.setTextSize(1);
        display.setCursor(0, 30);
        display.println(F("Main Page Content"));
    }
}