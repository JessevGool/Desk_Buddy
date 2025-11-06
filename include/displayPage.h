#pragma once
#include <string>
#include <Adafruit_ILI9341.h>

namespace DeskBuddy
{
    class DisplayPage
    {
    public:
        explicit DisplayPage(std::string name) : pageName(std::move(name)) {}
        virtual ~DisplayPage() {}

        const std::string &getName() const;

        virtual void draw(Adafruit_ILI9341 &display) = 0;

    private:
        std::string pageName;

    protected:
        void drawTitle(Adafruit_ILI9341 &display);
    };
};