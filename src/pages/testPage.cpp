#include "pages/testPage.h"
#include <vector>
#include "bitMapHelpers.h"
namespace DeskBuddy
{
    TestPage::TestPage(Adafruit_ILI9341 &display)
        : DisplayPage("Test", display)
    {
        this->gridConfig.rows = 10;
        this->gridConfig.cols = 7;
    }

    void TestPage::draw()
    {
        drawGrid();
        this->drawText("TEST", highlightedCol, highlightedRow);
        // drawIconCentered(this->display, sunny, 32, 32, ILI9341_YELLOW);
    }

    void TestPage::handleAction()
    {
        highlightedCol = (highlightedCol + 1) % this->gridConfig.cols;
    }

    void TestPage::handleUpAction()
    {
        highlightedRow = (highlightedRow - 1 + this->gridConfig.rows) % this->gridConfig.rows;
    }
    void TestPage::handleDownAction()
    {
        highlightedRow = (highlightedRow + 1) % this->gridConfig.rows;
    }

    void TestPage::highlightGridCell(int row, int col)
    {
        double cellWidth = static_cast<double>(this->display.width()) / this->gridConfig.cols;
        double cellHeight = static_cast<double>(this->display.height()) / this->gridConfig.rows;

        int x = static_cast<int>(col * cellWidth);
        int y = static_cast<int>(row * cellHeight);
        int w = static_cast<int>(cellWidth);
        int h = static_cast<int>(cellHeight);

        this->display.fillRect(x, y, w, h, ILI9341_GREEN);

        this->display.setTextColor(ILI9341_BLACK, ILI9341_GREEN);
        this->display.setTextSize(2);
        // Measure text bounds
        int16_t x1, y1;
        uint16_t tw, th;
        this->display.getTextBounds("TEST", 0, 0, &x1, &y1, &tw, &th);

        // Center in cell
        int textX = x + (w - tw) / 2;
        int textY = y + (h - th) / 2;
        this->display.setCursor(textX, textY);
        this->display.println("TEST");
    }
    /**
     * Draws a grid layout on the display with the specified number of rows and columns.
     * Each cell in the grid is outlined for visual separation.
     */
    void TestPage::drawGrid()
    {
        double cellWidth = static_cast<double>(this->display.width()) / this->gridConfig.cols;
        double cellHeight = static_cast<double>(this->display.height()) / this->gridConfig.rows;

        for (int r = 0; r < this->gridConfig.rows; ++r)
        {
            for (int c = 0; c < this->gridConfig.cols; ++c)
            {
                int x = static_cast<int>(c * cellWidth);
                int y = static_cast<int>(r * cellHeight);
                int w = static_cast<int>(cellWidth);
                int h = static_cast<int>(cellHeight);

                int bottomRightX = static_cast<int>((c + 1) * cellWidth);
                int bottomRightY = static_cast<int>((r + 1) * cellHeight);

                this->display.drawRect(x, y, w, h, C_SUBTEXT);
            }
        }
    }

}
