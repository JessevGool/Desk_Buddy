#include "controllers/joystickController.h"

namespace DeskBuddy
{
    JoystickController::JoystickController(int horizPin, int vertPin, int selectPin)
        : _horizPin(horizPin), _vertPin(vertPin), _selectPin(selectPin)
    {
        pinMode(_vertPin, INPUT);
        pinMode(_horizPin, INPUT);
        pinMode(_selectPin, INPUT_PULLUP);
    }

    JoystickState JoystickController::readState()
    {
        JoystickState state;
        state.horizontal = analogRead(_horizPin);
        state.vertical = analogRead(_vertPin);
        state.selectPressed = digitalRead(_selectPin) == LOW;
        state.direction = getCurrentDirection(state.horizontal, state.vertical);
        state.directionHeld = (state.direction != JoystickDirection::NONE) &&
                              (joystickHoldRight() || joystickHoldLeft() ||
                               joystickHoldUp() || joystickHoldDown());
        return state;
    }

    JoystickDirection JoystickController::getCurrentDirection(int horiz, int vert)
    {
        int horizDelta = horiz - ADC_CENTER;
        int vertDelta = vert - ADC_CENTER;

        // Check if movement is significant enough
        if (abs(horizDelta) < DEADZONE && abs(vertDelta) < DEADZONE)
        {
            return JoystickDirection::NONE;
        }

        // Determine primary direction (favor horizontal over vertical for equal values)
        if (abs(horizDelta) >= abs(vertDelta))
        {
            if (horizDelta >= DIRECTION_OFFSET)
                return JoystickDirection::RIGHT;
            if (horizDelta <= -DIRECTION_OFFSET)
                return JoystickDirection::LEFT;
        }
        else
        {
            if (vertDelta >= DIRECTION_OFFSET)
                return JoystickDirection::DOWN;
            if (vertDelta <= -DIRECTION_OFFSET)
                return JoystickDirection::UP;
        }

        return JoystickDirection::NONE;
    }

    bool JoystickController::checkHoldDirection(bool &active, bool &fired, unsigned long &startMs, bool inDirection)
    {
        if (inDirection)
        {
            if (!active)
            {
                active = true;
                startMs = millis();
                fired = false;
            }
            else
            {
                if (!fired && (millis() - startMs >= HOLD_MS))
                {
                    fired = true;
                    return true;
                }
            }
        }
        else
        {
            active = false;
            fired = false;
        }
        return false;
    }

    bool JoystickController::joystickHoldRight()
    {
        int delta = analogRead(_horizPin) - ADC_CENTER;
        bool inRight = (delta >= (DEADZONE + DIRECTION_OFFSET));
        return checkHoldDirection(rightActive, rightFired, rightStartMs, inRight);
    }

    bool JoystickController::joystickHoldLeft()
    {
        int delta = analogRead(_horizPin) - ADC_CENTER;
        bool inLeft = (delta <= -(DEADZONE + DIRECTION_OFFSET));
        return checkHoldDirection(leftActive, leftFired, leftStartMs, inLeft);
    }

    bool JoystickController::joystickHoldUp()
    {
        int delta = analogRead(_vertPin) - ADC_CENTER;
        bool inUp = (delta <= -(DEADZONE + DIRECTION_OFFSET));
        return checkHoldDirection(upActive, upFired, upStartMs, inUp);
    }

    bool JoystickController::joystickHoldDown()
    {
        int delta = analogRead(_vertPin) - ADC_CENTER;
        bool inDown = (delta >= (DEADZONE + DIRECTION_OFFSET));
        return checkHoldDirection(downActive, downFired, downStartMs, inDown);
    }

    bool JoystickController::isRightActive(int horizValue)
    {
        return (horizValue - ADC_CENTER) >= (DEADZONE + DIRECTION_OFFSET);
    }

    bool JoystickController::isLeftActive(int horizValue)
    {
        return (horizValue - ADC_CENTER) <= -(DEADZONE + DIRECTION_OFFSET);
    }

    bool JoystickController::isUpActive(int vertValue)
    {
        return (vertValue - ADC_CENTER) <= -(DEADZONE + DIRECTION_OFFSET);
    }

    bool JoystickController::isDownActive(int vertValue)
    {
        return (vertValue - ADC_CENTER) >= (DEADZONE + DIRECTION_OFFSET);
    }

    bool JoystickController::isSelectPressed()
    {
        bool currentState = digitalRead(_selectPin) == LOW;
        bool pressed = currentState && !_prevSelectState;
        _prevSelectState = currentState;
        return pressed;
    }
}