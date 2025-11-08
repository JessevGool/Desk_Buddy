#include "pages/secondPage.h"
#include "bitMapHelpers.h"
namespace DeskBuddy
{
    void SecondPage::draw(Adafruit_ILI9341 &display)
    {
        for (const uint16_t *img : images)
        {
            drawImageCentered(display, img, 128, 128);
            delay(100);
        }
    }
}
