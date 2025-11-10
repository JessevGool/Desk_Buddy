#include "pages/secondPage.h"
#include "bitMapHelpers.h"
namespace DeskBuddy
{
    void SecondPage::draw()
    {
        if (displayImage)
        {
            for (const uint16_t *img : images)
            {
                if (!displayImage)
                {
                    break;
                }

                drawImageCentered(this->display, img, 128, 128);
                delay(100);
            }
        }
        else
        {
            this->display.drawLine(40, 200, 100, 100, ILI9341_WHITE);
            this->display.drawLine(100, 100, 160, 200, ILI9341_WHITE);
            this->display.drawLine(160, 200, 40, 200, ILI9341_WHITE);
        }
    }

    void SecondPage::handleAction()
    {
        displayImage = !displayImage;
        this->display.fillScreen(ILI9341_BLACK);
        delay(200);
    }
}
