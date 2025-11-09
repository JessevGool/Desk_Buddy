#include "pages/mainPage.h"

namespace DeskBuddy
{
    void MainPage::draw()
    {
        drawTitle(this->display);
        this->display.setTextSize(1);
        this->display.setCursor(0, 30);
        this->display.println(F("Main Page Content"));
        this->display.println(F("Joystick controls:"));
        this->display.println(F(" - Right: Next page"));
        this->display.println(F(" - Left: Previous page"));
        this->display.println(F(" - Select: Action"));

        // You can add more content here like sensor readings
        this->display.setCursor(0, 80);
        this->display.println(F("Status: Ready"));
    }

    void MainPage::handleAction()
    {
        // Example action: could toggle a setting or refresh data
        // For now, just print to serial
        Serial.println("MainPage action triggered!");
    }
}