#pragma once
#include <string>
#include <Adafruit_ILI9341.h>

namespace DeskBuddy
{

    struct GridConfig
    {
        int rows;
        int cols;
    };

    class DisplayPage
    {
    public:
        explicit DisplayPage(std::string name, Adafruit_ILI9341 &display) : pageName(std::move(name)), display(display) {}
        virtual ~DisplayPage() {}

        const std::string &getName() const;

        virtual void draw() = 0;
        virtual void handleAction() = 0;
        virtual void handleDownAction() = 0;
        virtual void handleUpAction() = 0;
        virtual void onActivate();
        virtual void onDeactivate();

    private:
        std::string pageName;

        void clearGridPosition(int col, int row);

    protected:
        void drawTitle(Adafruit_ILI9341 &display);
        void drawText(const char* text, int col, int row, uint8_t textSize = 1, uint16_t color = ILI9341_WHITE, uint16_t bgColor = ILI9341_BLACK);
        void drawIcon(const uint8_t* bitmap, int bmpWidth, int bmpHeight, int col, int row);
        Adafruit_ILI9341 display;
        
        GridConfig gridConfig{1,1};

        bool redrawIcon = true;
    };
};