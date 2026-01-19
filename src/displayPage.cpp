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

    void DisplayPage::onActivate()
    {
        // Default implementation does nothing
    }

    void DisplayPage::onDeactivate()
    {
        // Default implementation does nothing
    }

    void DisplayPage::drawText(const char *text, int col, int row, uint8_t textSize, uint16_t color, uint16_t bgColor)
    {
        if (col < 0 || col >= this->gridConfig.cols || row < 0 || row >= this->gridConfig.rows)
        {
            return;
        }
        double cellWidth = static_cast<double>(this->display.width()) / this->gridConfig.cols;
        double cellHeight = static_cast<double>(this->display.height()) / this->gridConfig.rows;

        int x = static_cast<int>(col * cellWidth);
        int y = static_cast<int>(row * cellHeight);

        this->display.setTextColor(color, bgColor);
        this->display.setTextSize(textSize);

        String str(text);
        int lineNum = 0;
        int startIdx = 0;

        while (startIdx < str.length())
        {
            int endIdx = str.indexOf('\n', startIdx);
            if (endIdx == -1)
            {
                endIdx = str.length();
            }

            String line = str.substring(startIdx, endIdx);

            int16_t x1, y1;
            uint16_t tw, th;
            this->display.getTextBounds(line.c_str(), 0, 0, &x1, &y1, &tw, &th);

            int textX = x + (cellWidth - tw) / 2;
            int textY = y + (lineNum * (th + 2));
            this->display.setCursor(textX, textY);
            this->display.print(line);

            lineNum++;
            startIdx = endIdx + 1;
        }
    }

    void DisplayPage::drawIcon(const uint8_t* bitmap, int bmpWidth, int bmpHeight, int col, int row)
    {

        this->clearGridPosition(col, row);
        if (col < 0 || col >= this->gridConfig.cols || row < 0 || row >= this->gridConfig.rows)
        {
            return;
        }
        double cellWidth = static_cast<double>(this->display.width()) / this->gridConfig.cols;
        double cellHeight = static_cast<double>(this->display.height()) / this->gridConfig.rows;

        int x = static_cast<int>(col * cellWidth + (cellWidth - bmpWidth) / 2);
        int y = static_cast<int>(row * cellHeight + (cellHeight - bmpHeight) / 2);

        this->display.drawBitmap(x, y, bitmap, bmpWidth, bmpHeight, ILI9341_WHITE);
    }

    void DisplayPage::clearGridPosition(int col, int row)
    {
        if (col < 0 || col >= this->gridConfig.cols || row < 0 || row >= this->gridConfig.rows)
        {
            return;
        }
        double cellWidth = static_cast<double>(this->display.width()) / this->gridConfig.cols;
        double cellHeight = static_cast<double>(this->display.height()) / this->gridConfig.rows;

        int x = static_cast<int>(col * cellWidth);
        int y = static_cast<int>(row * cellHeight);

        this->display.fillRect(x, y, static_cast<int>(cellWidth), static_cast<int>(cellHeight), ILI9341_BLACK);
    }
}