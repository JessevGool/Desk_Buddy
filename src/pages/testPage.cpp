#include "pages/testPage.h"
#include <vector>

namespace DeskBuddy
{

    struct PixelData
    {
        int x, y;
        uint16_t color;
        unsigned long timestamp;
    };
    std::vector<PixelData> drawnPixels;
    const unsigned long PIXEL_LIFETIME_MS = 1000; // 1 seconds
    static int pixelColorCounter = 0;

    void TestPage::draw()
    {

        struct tm timeinfo;
        if (getLocalTime(&timeinfo))
        {
            char timeStr[32];
            strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);

            // Draw time in top-left corner
            this->display.setTextColor(C_TEXT, ILI9341_BLACK);
            this->display.setTextSize(3);
            this->display.setCursor(10, 10);
            this->display.print(timeStr);

            // Or display date too
            char dateStr[32];
            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &timeinfo);
            this->display.setTextSize(2);
            this->display.setCursor(10, 40);
            this->display.println(dateStr);
        }

        this->display.printf("Free heap: %d\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));
        this->display.printf("Largest block: %d\n", heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));

        drawGrid(1, gridSize);
    }

    void TestPage::handleAction()
    {
        // this->gridSize += 1;
        // // Serial.println("TestPage action triggered!");
        // drawnPixels.clear();
    }

    void TestPage::drawCard(int x, int y, int w, int h, const char *title)
    {

        this->display.fillRoundRect(x, y, w, h, 10, C_CARD);

        this->display.fillRoundRect(x, y, w, 6, 10, C_ACCENT);

        this->display.setTextColor(C_SUBTEXT);
        this->display.setTextSize(2);
        this->display.setCursor(x + 10, y + 12);
        this->display.print(title);
    }

    /**
     * Draws a grid layout on the display with the specified number of rows and columns.
     * Each cell in the grid is outlined for visual separation.
     */

    void TestPage::drawGrid(int rows, int cols)
    {
        double cellWidth = static_cast<double>(this->display.width()) / cols;
        double cellHeight = static_cast<double>(this->display.height()) / rows;

        static unsigned long lastTime = 0;
        unsigned long currentTime = millis();
        unsigned long elapsed = currentTime - lastTime;
        lastTime = currentTime;

        static float animationOffset = 0;
        animationOffset += elapsed * 0.1f;

        for (auto it = drawnPixels.begin(); it != drawnPixels.end();)
        {
            if (currentTime - it->timestamp > PIXEL_LIFETIME_MS)
            {

                this->display.drawPixel(it->x, it->y, C_BG);
                it = drawnPixels.erase(it);
            }
            else
            {
                this->display.drawPixel(it->x, it->y, it->color);
                ++it;
            }
        }

        for (int r = 0; r < rows; ++r)
        {
            for (int c = 0; c < cols; ++c)
            {
                int x = static_cast<int>(c * cellWidth);
                int y = static_cast<int>(r * cellHeight);
                int w = static_cast<int>(cellWidth);
                int h = static_cast<int>(cellHeight);

                int bottomRightX = static_cast<int>((c + 1) * cellWidth);
                int bottomRightY = static_cast<int>((r + 1) * cellHeight);

                this->display.drawRect(x, y, w, h, C_SUBTEXT);

                // this->drawSun(x,y,w,h);

                if (drawnPixels.size() < 100)
                {
                    int randomX = random(x, bottomRightX);
                    int randomY = random(y, bottomRightY);
                    uint16_t color = (pixelColorCounter % 2 == 0) ? C_FIRST_PIXELS : C_SECOND_PIXELS;
                    pixelColorCounter++;

                    drawnPixels.push_back({randomX, randomY, color, currentTime});
                }

                // this->display.setTextSize(8);
                // this->display.setTextColor(C_TEXT);
                // this->display.setCursor(x + w / 2 - 10, y + h / 2 - 16);
                // this->display.print((r * cols) + c + 1);
            }
        }
    }

    void TestPage::drawSun(int x, int y, int w, int h)
    {
        int centerX = x + w / 2;
        int centerY = y + h / 2;
        int radius = (w < h ? w : h) / 6;

        this->display.drawCircle(centerX, centerY, radius, ILI9341_WHITE);

        int lines = 8;

        for (int i = 0; i < lines; ++i)
        {
            float angle = (2 * 3.14159f / lines) * i;
            int xStart = centerX + static_cast<int>(radius * cos(angle));
            int yStart = centerY + static_cast<int>(radius * sin(angle));
            int xEnd = centerX + static_cast<int>((radius + 10) * cos(angle));
            int yEnd = centerY + static_cast<int>((radius + 10) * sin(angle));

            this->display.drawLine(xStart, yStart, xEnd, yEnd, ILI9341_WHITE);
        }
    }
}
