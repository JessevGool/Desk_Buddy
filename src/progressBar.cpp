#include "progressBar.h"

namespace DeskBuddy
{
    ProgressBar::ProgressBar(Adafruit_ILI9341& tft, int x, int y, int width, int height)
        : tft(tft), x(x), y(y), width(width), height(height), progress(0) {}

    ProgressBar::ProgressBar(Adafruit_ILI9341& tft, int x, int y, int width, int height, int progress)
        : ProgressBar(tft, x, y, width, height)
    {
        setProgress(progress);
    }

    void ProgressBar::setProgress(int progress)
    {
        if (progress < 0)
            this->progress = 0;
        else if (progress > 100)
            this->progress = 100;
        else
            this->progress = progress;
    }

    void ProgressBar::draw()
    {
        this->tft.drawRect(x, y, width, height, ILI9341_WHITE);
        int filledWidth = (width - 2) * progress / 100;
        tft.fillRect(x + 1, y + 1, filledWidth, height - 2, ILI9341_GREEN);
        this->tft.fillRect(x + 1 + filledWidth, y + 1, width - 2 - filledWidth, height - 2, ILI9341_BLACK);
    }
}