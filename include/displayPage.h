#pragma once
#include <string>
#include <Adafruit_ILI9341.h>

namespace DeskBuddy
{
    class DisplayPage
    {
    public:
        explicit DisplayPage(std::string name, Adafruit_ILI9341 &display) : pageName(std::move(name)), display(display) {}
        virtual ~DisplayPage() {}

        const std::string &getName() const;

        virtual void draw() = 0;
        virtual void handleAction() = 0;

    private:
        std::string pageName;

    protected:
        void drawTitle(Adafruit_ILI9341 &display);
        Adafruit_ILI9341 display;
    };
};