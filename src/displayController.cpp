#include "DisplayController.h"
#include <stdexcept>
#include <utility> // for std::make_pair

namespace DeskBuddy
{

    DisplayController::DisplayController() : currentIndex(0)
    {
        DeskBuddy::DisplayPage mainPage = DeskBuddy::DisplayPage("Main");
        DeskBuddy::DisplayPage secondPage = DeskBuddy::DisplayPage("Second");

        this->AddPage(mainPage);
        this->AddPage(secondPage);
    }
    void DisplayController::AddPage(const DisplayPage &page)
    {
        const std::string &name = page.GetName();

        // Look for an existing page with the same name
        for (std::vector<std::pair<std::string, DisplayPage>>::iterator it = pages.begin();
             it != pages.end();
             ++it)
        {
            if (it->first == name)
            {
                it->second = page; // overwrite
                return;
            }
        }

        // Not found -> append
        pages.push_back(std::make_pair(name, page));
        if (pages.size() == 1)
        {
            currentIndex = 0; // init on first insert
        }
    }

    const DisplayPage &DisplayController::GetPage(const std::string &name) const
    {
        for (std::vector<std::pair<std::string, DisplayPage>>::const_iterator it = pages.begin();
             it != pages.end();
             ++it)
        {
            if (it->first == name)
            {
                return it->second;
            }
        }
        throw std::out_of_range("Page not found: " + name);
    }

    DisplayPage &DisplayController::GetPage(const std::string &name)
    {
        // Reuse const overload without auto
        const DisplayController *selfConst = this;
        const DisplayPage &cref = selfConst->GetPage(name);
        return const_cast<DisplayPage &>(cref);
    }

    DisplayPage &DisplayController::GetNextPage()
    {
        if (pages.empty())
        {
            throw std::runtime_error("No pages available");
        }
        currentIndex = (currentIndex + 1) % pages.size();
        return pages[currentIndex].second;
    }

    bool DisplayController::HasPage(const std::string &name) const
    {
        for (std::vector<std::pair<std::string, DisplayPage>>::const_iterator it = pages.begin();
             it != pages.end();
             ++it)
        {
            if (it->first == name)
            {
                return true;
            }
        }
        return false;
    }

} // namespace DeskBuddy
