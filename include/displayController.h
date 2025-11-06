#pragma once
#include <vector>
#include <string>
#include <memory>
#include <Adafruit_ILI9341.h>
#include "displayPage.h"
#include "pages/mainPage.h"
#include "pages/secondPage.h"

namespace DeskBuddy
{
    class DisplayController
    {
    public:
        DisplayController() = default;
        DisplayController(Adafruit_ILI9341 &display);

        // Add without triggering default construction
        void addPage(std::unique_ptr<DisplayPage> page);

        // Return a reference to avoid copies and avoid default construction
        const DisplayPage &getPage(const std::string &name) const;
        DisplayPage &getNextPage();
        // Optional: non-const accessor
        DisplayPage &getPage(const std::string &name);

        // Optional: check existence
        bool hasPage(const std::string &name) const;
        void drawCurrentPage();

    private:
        std::vector<std::pair<std::string, std::unique_ptr<DisplayPage>>> pages;

        size_t currentIndex = 0;
        Adafruit_ILI9341 display;
    };
}
