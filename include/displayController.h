#pragma once
#include <vector>
#include <string>
#include "DisplayPage.h"

namespace DeskBuddy
{
    class DisplayController
    {
    public:
        DisplayController();

        // Add without triggering default construction
        void AddPage(const DisplayPage &page);

        // Return a reference to avoid copies and avoid default construction
        const DisplayPage &GetPage(const std::string &name) const;
        DisplayPage &GetNextPage();
        // Optional: non-const accessor
        DisplayPage &GetPage(const std::string &name);

        // Optional: check existence
        bool HasPage(const std::string &name) const;

    private:
        std::vector<std::pair<std::string, DisplayPage>> pages;

        size_t currentIndex = 0;
    };
}
