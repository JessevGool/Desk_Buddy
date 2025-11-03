#pragma once
#include <unordered_map>
#include <string>
#include "DisplayPage.h"

namespace DeskBuddy
{
    class DisplayController
    {
    public:
        DisplayController() = default;

        // Add without triggering default construction
        void AddPage(const DisplayPage& page);

        // Return a reference to avoid copies and avoid default construction
        const DisplayPage& GetPage(const std::string& name) const;

        // Optional: non-const accessor
        DisplayPage& GetPage(const std::string& name);

        // Optional: check existence
        bool HasPage(const std::string& name) const;

    private:
        std::unordered_map<std::string, DisplayPage> pages;
    };
}
