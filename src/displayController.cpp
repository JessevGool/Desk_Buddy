#include "DisplayController.h"
#include <stdexcept>

namespace DeskBuddy
{
    void DisplayController::AddPage(const DisplayPage& page)
    {
        // emplace won't default-construct value; it constructs in-place
        pages.emplace(page.GetName(), page);
        // If you want to overwrite if it exists, use:
        // pages.insert_or_assign(page.GetName(), page); // C++17
    }

    const DisplayPage& DisplayController::GetPage(const std::string& name) const
    {
        // at() does NOT create a new value; it throws if missing
        return pages.at(name);
    }

    DisplayPage& DisplayController::GetPage(const std::string& name)
    {
        return pages.at(name);
    }

    bool DisplayController::HasPage(const std::string& name) const
    {
        return pages.find(name) != pages.end();
    }
}
