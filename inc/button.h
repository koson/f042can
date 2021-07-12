#ifndef BUTTON_H
#define BUTTON_H

//#include "can.h"
#include "stm32f0xx_hal.h"

enum class ButtonState {
    NOT_PRESSED = 0,
    UP,
    DOWN,
    UP_LONG,
    DOWN_LONG,
    UP_DOWN
};

class Button {
public:
    Button();
    static Button* pThis;
    ButtonState butState = ButtonState::NOT_PRESSED;
private:
    void init();
};

#endif //BUTTON_H