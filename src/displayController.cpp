#include "displayController.h"
#include <stdexcept>
#include <utility> // for std::make_pair

namespace DeskBuddy
{

    DisplayController::DisplayController(Adafruit_ILI9341 &display) : display(display), currentIndex(0)
    {
        this->addPage(std::unique_ptr<DisplayPage>(new MainPage()));
        this->addPage(std::unique_ptr<DisplayPage>(new SecondPage()));
    }

    void DisplayController::addPage(std::unique_ptr<DisplayPage> page)
    {
        const std::string &name = page->getName();

        // Look for an existing page with the same name
        for (auto it = pages.begin(); it != pages.end(); ++it)
        {
            if (it->first == name)
            {
                it->second = std::move(page); // overwrite
                return;
            }
        }

        // Not found -> append
        pages.push_back(std::make_pair(name, std::move(page)));
        if (pages.size() == 1)
        {
            currentIndex = 0; // init on first insert
        }
    }

    const DisplayPage &DisplayController::getPage(const std::string &name) const
    {
        for (auto it = pages.begin(); it != pages.end(); ++it)
        {
            if (it->first == name)
            {
                return *(it->second);
            }
        }
        throw std::out_of_range("Page not found: " + name);
    }

    DisplayPage &DisplayController::getPage(const std::string &name)
    {
        for (auto it = pages.begin(); it != pages.end(); ++it)
        {
            if (it->first == name)
            {
                return *(it->second);
            }
        }
        throw std::out_of_range("Page not found: " + name);
    }

    DisplayPage &DisplayController::getNextPage()
    {
        if (pages.empty())
        {
            throw std::runtime_error("No pages available");
        }
        currentIndex = (currentIndex + 1) % pages.size();
        return *(pages[currentIndex].second);
    }

    bool DisplayController::hasPage(const std::string &name) const
    {
        for (auto it = pages.begin(); it != pages.end(); ++it)
        {
            if (it->first == name)
            {
                return true;
            }
        }
        return false;
    }

    void DisplayController::drawCurrentPage()
    {
        this->display.setCursor(0, 0);
        this->display.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        if (pages.empty())
        {
            throw std::runtime_error("No pages available to draw");
        }
        pages[currentIndex].second->draw(this->display);
    }

} // namespace DeskBuddy
