#include "bitMapHelpers.h"

void drawImageCentered(Adafruit_ILI9341 &display, const uint16_t *img, int w, int h)
{
    int16_t x = 0;
    int16_t y = 0;

    if (w < display.width())
        x = (display.width() - w) / 2;
    if (h < display.height())
        y = (display.height() - h) / 2;

    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;

    display.drawRGBBitmap(x, y, img, w, h);
}

void drawIconCentered(Adafruit_ILI9341 &display, const uint8_t *bmp, int w, int h, uint16_t color)
{
    int16_t x = 0;
    int16_t y = 0;

    if (w < display.width())
        x = (display.width() - w) / 2;
    if (h < display.height())
        y = (display.height() - h) / 2;

    display.drawBitmap(x, y, bmp, w, h, color);
}