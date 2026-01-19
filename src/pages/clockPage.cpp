#include "pages/clockPage.h"

namespace DeskBuddy
{

    void ClockPage::draw()
    {
        struct tm timeinfo;
        if (getLocalTime(&timeinfo))
        {
            int textSize = 3;
            char timeStr[32];
            strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);

            // Draw time centered horizontally
            this->display.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
            this->display.setTextSize(textSize);

            int textWidth = strlen(timeStr) * 18; // ~6 * textSize(3)
            int xPos = (this->display.width() - textWidth) / 2;
            int yPos = ((this->display.height() - (8 * textSize)) / 2) - 20; 

            this->display.setCursor(xPos, yPos);
            this->display.print(timeStr);

            // Or display date too
            char dateStr[32];
            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &timeinfo);
            this->display.setTextSize(2);

            int dateWidth = strlen(dateStr) * 12; // ~6 * textSize(2)
            xPos = (this->display.width() - dateWidth) / 2;
            yPos += (8 * textSize) + 10; // Move below time


            this->display.setCursor(xPos, yPos);
            this->display.println(dateStr);
        }
    }

    void ClockPage::handleAction()
    {
    }
}