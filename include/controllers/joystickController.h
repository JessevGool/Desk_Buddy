#pragma once
#include <Arduino.h>

namespace DeskBuddy
{
    enum class JoystickDirection {
        NONE,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    
    struct JoystickState {
        int horizontal;
        int vertical;
        bool selectPressed;
        JoystickDirection direction;
        bool directionHeld;
    };

    class JoystickController
    {
    public:
        JoystickController(int horizPin, int vertPin, int selectPin);
        
        // Read current state
        JoystickState readState();
        
        // Individual direction checks
        bool isRightActive(int horizValue);
        bool isLeftActive(int horizValue);
        bool isUpActive(int vertValue);
        bool isDownActive(int vertValue);
        bool isSelectPressed(int selectValue);
        
        // Hold detection
        bool joystickHoldRight();
        bool joystickHoldLeft();
        bool joystickHoldUp();
        bool joystickHoldDown();
        
        // Get raw values
        int getHorizontal() { return analogRead(_horizPin); }
        int getVertical() { return analogRead(_vertPin); }
        bool getSelect() { return digitalRead(_selectPin) == LOW; }

    private:
        int _horizPin;
        int _vertPin;
        int _selectPin;

        const int ADC_CENTER = 2048;       // ESP32 12-bit ADC ~ center
        const int DEADZONE = 300;          // ignore small wiggles
        const int DIRECTION_OFFSET = 800;  // how far from center counts as direction
        const unsigned long HOLD_MS = 500; // hold duration to trigger (ms)

        // Direction tracking
        bool rightActive = false;
        bool leftActive = false;
        bool upActive = false;
        bool downActive = false;
        
        bool rightFired = false;
        bool leftFired = false;
        bool upFired = false;
        bool downFired = false;
        
        unsigned long rightStartMs = 0;
        unsigned long leftStartMs = 0;
        unsigned long upStartMs = 0;
        unsigned long downStartMs = 0;
        
        JoystickDirection getCurrentDirection(int horiz, int vert);
        bool checkHoldDirection(bool &active, bool &fired, unsigned long &startMs, bool inDirection);
    };
}