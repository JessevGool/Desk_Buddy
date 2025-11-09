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
                if (!displayImage) {
                    break;
                }
            
                drawImageCentered(this->display, img, 128, 128);
                delay(100);
            }
        }
    }

    void SecondPage::handleAction()
    {
        displayImage = !displayImage;
        this->display.fillScreen(ILI9341_BLACK);
        delay(200);
    }
}
