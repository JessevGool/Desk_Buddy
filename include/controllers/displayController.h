#pragma once
#include <vector>
#include <string>
#include <memory>
#include <Adafruit_ILI9341.h>
#include "displayPage.h"
#include "controllers/joystickController.h"
#include "apiClient.h"

#include "pages/mainPage.h"
#include "pages/secondPage.h"
#include "pages/minecraftServerInfoPage.h"
#include "pages/weatherPage.h"
#include "pages/testPage.h"

namespace DeskBuddy
{
    class DisplayController
    {
    public:
        DisplayController() = default;
        DisplayController(Adafruit_ILI9341 &display, JoystickController &joystick);

        // Page management
        void addPage(std::unique_ptr<DisplayPage> page);
        const DisplayPage &getPage(const std::string &name) const;
        DisplayPage &getNextPage();
        DisplayPage &getPreviousPage();
        DisplayPage &getPage(const std::string &name);
        bool hasPage(const std::string &name) const;

        // Display and input handling
        void update(); // Call this in loop() - handles input and drawing
        void drawCurrentPage();

        // Input handling
        void setJoystickController(JoystickController &joystick);

    private:
        void setup();
        static void InputTaskTrampoline(void *param);
        void InputTaskLoop();
        std::vector<std::pair<std::string, std::unique_ptr<DisplayPage>>> pages;

        size_t currentIndex = 0;
        Adafruit_ILI9341 display;
        JoystickController *joystickController = nullptr;

        // Input state tracking
        bool needsRedraw = true;
        unsigned long lastInputTime = 0;
        static const unsigned long INPUT_DEBOUNCE_MS = 50;

        TaskHandle_t inputTaskHandle = nullptr;
    };
}
