#include "pages/secondPage.h"

namespace DeskBuddy
{
    void SecondPage::draw(Adafruit_ILI9341 &display)
    {
        drawTitle(display);
        display.setTextSize(1);
        display.setCursor(0, 30);
        display.println(F("Second Page Content"));
        display.println(F("This is the second page!"));
        display.println(F("Use joystick right to switch"));
        display.println(F("back to Main page."));
    }
}
