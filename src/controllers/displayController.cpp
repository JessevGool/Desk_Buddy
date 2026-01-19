#include "controllers/displayController.h"
#include <stdexcept>
#include <utility> // for std::make_pair

namespace DeskBuddy
{

    DisplayController::DisplayController(Adafruit_ILI9341 &display, JoystickController &joystick)
        : display(display), currentIndex(0), joystickController(&joystick)
    {
        this->setup();
    }

    void DisplayController::setup()
    {
        g_httpMutex = xSemaphoreCreateMutex();
        xTaskCreatePinnedToCore(
            InputTaskTrampoline,
            "Display Input Task",
            4096,
            this,
            1,
            &inputTaskHandle,
            1);
        ApiClient &client = DeskBuddy::ApiClient::instance();

        this->addPage(std::unique_ptr<DisplayPage>(new MainPage(display)));
        this->addPage(std::unique_ptr<DisplayPage>(new ClockPage(display)));
        //this->addPage(std::unique_ptr<DisplayPage>(new SecondPage(display)));
        this->addPage(std::unique_ptr<DisplayPage>(new MinecraftServerInfoPage(client, display)));
        this->addPage(std::unique_ptr<DisplayPage>(new WeatherPage(client, display)));
        this->addPage(std::unique_ptr<DisplayPage>(new TestPage(display)));
        
        pages[currentIndex].second->onActivate();
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
        pages[currentIndex].second->onDeactivate();

        currentIndex = (currentIndex + 1) % pages.size();
        needsRedraw = true;
        pages[currentIndex].second->onActivate();

        return *(pages[currentIndex].second);
    }

    DisplayPage &DisplayController::getPreviousPage()
    {
        if (pages.empty())
        {
            throw std::runtime_error("No pages available");
        }
        pages[currentIndex].second->onDeactivate();

        currentIndex = (currentIndex == 0) ? pages.size() - 1 : currentIndex - 1;
        needsRedraw = true;
        pages[currentIndex].second->onActivate();

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
        if (pages.empty())
        {
            throw std::runtime_error("No pages available to draw");
        }

        if (needsRedraw)
        {
            display.fillScreen(ILI9341_BLACK);
            needsRedraw = false;
        }

        display.setCursor(0, 0);
        display.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        pages[currentIndex].second->draw();
    }

    void DisplayController::setJoystickController(JoystickController &joystick)
    {
        joystickController = &joystick;
    }

    void DisplayController::InputTaskTrampoline(void *param)
    {
        auto *self = static_cast<DisplayController *>(param);
        self->InputTaskLoop();
        vTaskDelete(nullptr); // just in case loop exits
    }

    void DisplayController::InputTaskLoop()
    {
        for (;;)
        {
            if (!joystickController)
            {
                vTaskDelay(pdMS_TO_TICKS(50));
                continue;
            }

            unsigned long currentTime = millis();
            if (currentTime - lastInputTime < INPUT_DEBOUNCE_MS)
            {
                vTaskDelay(pdMS_TO_TICKS(5));
                continue;
            }

            if (joystickController->joystickHoldRight())
            {
                getNextPage();
                lastInputTime = currentTime;
                Serial.printf("Switched to next page: %s\n", pages[currentIndex].first.c_str());
            }
            else if (joystickController->joystickHoldLeft())
            {
                getPreviousPage();
                lastInputTime = currentTime;
                Serial.printf("Switched to previous page: %s\n", pages[currentIndex].first.c_str());
            }

            if (joystickController->isSelectPressed())
            {
                pages[currentIndex].second->handleAction();
                needsRedraw = true;
                lastInputTime = currentTime;
            }

            if(joystickController->joystickHoldUp()) {
                pages[currentIndex].second->handleUpAction();
                 needsRedraw = true;
                lastInputTime = currentTime;
            }

             if(joystickController->joystickHoldDown()) {
                pages[currentIndex].second->handleDownAction();
                 needsRedraw = true;
                lastInputTime = currentTime;
            }
            // avoid a hot loop
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }

    void DisplayController::update()
    {
        drawCurrentPage();
    }

} // namespace DeskBuddy
