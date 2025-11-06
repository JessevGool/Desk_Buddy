#include "displayPage.h"

namespace DeskBuddy
{
    const std::string &DisplayPage::getName() const
    {
        return pageName;
    }

    void DisplayPage::drawTitle(Adafruit_ILI9341 &display)
    {
        display.setTextSize(2);
        display.setCursor(0, 0);
        display.print(F("Current Page: "));
        display.println(this->getName().c_str());
    }
}