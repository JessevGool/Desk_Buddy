#pragma once

#include <Arduino.h>

#include "displayPage.h"

namespace DeskBuddy
{
    class TestPage : public DisplayPage
    {
    public:
        TestPage(Adafruit_ILI9341 &display);
        void draw() override;
        void handleAction() override;
                void handleUpAction() override;
        void handleDownAction() override;
        void drawCard(int x, int y, int w, int h, const char *title);
        void drawGrid();
        void drawSun(int x, int y, int w, int h);
        void highlightGridCell(int row, int col);

    private:
        const uint16_t C_BG = ILI9341_BLACK;
        const uint16_t C_CARD = 0x18E3; // dark gray-blue-ish
        const uint16_t C_TEXT = ILI9341_WHITE;
        const uint16_t C_SUBTEXT = 0xC618; // light gray

        const uint16_t C_FIRST_PIXELS = 0x77BD;
        const uint16_t C_SECOND_PIXELS = 0xE7BE;
        const uint16_t C_ACCENT = ILI9341_CYAN;
        bool firstRun = true;

        int randomPixels = 0;
        int highlightedRow = 0;
        int highlightedCol = 0;
    };

}