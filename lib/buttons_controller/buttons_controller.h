#pragma once
#include <Arduino.h>

#define DEBOUNCE_TIME_MILLISECONDS 20
#define BTN_LEFT D2
#define BTN_MID D1
#define BTN_RIGHT D0


struct ButtonsController {
    bool toggle_override_left;
    bool toggle_override_mid;
    bool toggle_override_right;

    bool override_left;
    bool override_mid;
    bool override_right;
};

void buttonsControllerInit();
void checkButtonsAndToggleOverride();
