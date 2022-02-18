#include "buttons_controller.h"


static bool isButtonPressed(uint8_t button);
static bool debounceButton(uint8_t button);


void buttonsControllerInit(struct ButtonsController *buttons_controller) {
    pinMode(BTN_LEFT, INPUT);
    pinMode(BTN_MID, INPUT);
    pinMode(BTN_RIGHT, INPUT);

    buttons_controller->override_left = false;
    buttons_controller->override_mid = false;
    buttons_controller->override_right = false;
}

void checkButtonsAndToggleOverride(struct ButtonsController *buttons_controller) {
    if (isButtonPressed(BTN_LEFT)) {
        overrideToggle(buttons_controller, BTN_LEFT);
    }

    if (isButtonPressed(BTN_MID)) {
        overrideToggle(buttons_controller, BTN_MID);
    }

    if (isButtonPressed(BTN_RIGHT)) {
        overrideToggle(buttons_controller, BTN_RIGHT);
    }
}

void overrideToggle(struct ButtonsController *buttons_controller, uint8_t button) {
    switch (button) {
        case BTN_LEFT:
            Serial.print("Toggled override for left hook.. Current status: ");
            if (buttons_controller->override_left) {
                overrideTurnOff(buttons_controller, BTN_LEFT);
            } else {
                overrideTurnOn(buttons_controller, BTN_LEFT);
            }
            Serial.println(buttons_controller->override_left);
            break;
        case BTN_MID:
            Serial.print("Toggled override for middle hook.. Current status: ");
            if (buttons_controller->override_mid) {
                overrideTurnOff(buttons_controller, BTN_MID);
            } else {
                overrideTurnOn(buttons_controller, BTN_MID);
            }
            Serial.println(buttons_controller->override_mid);
            break;
        case BTN_RIGHT:
            Serial.print("Toggled override for right hook.. Current status: ");
            if (buttons_controller->override_right) {
                overrideTurnOff(buttons_controller, BTN_RIGHT);
            } else {
                overrideTurnOn(buttons_controller, BTN_RIGHT);
            }
            Serial.println(buttons_controller->override_right);
            break;
    }
}

void overrideTurnOn(struct ButtonsController *buttons_controller, uint8_t button) {
    switch (button) {
        case BTN_LEFT:
            buttons_controller->override_left = true;
            break;
        case BTN_MID:
            buttons_controller->override_mid = true;
            break;
        case BTN_RIGHT:
            buttons_controller->override_right = true;
            break;
    }
}

void overrideTurnOff(struct ButtonsController *buttons_controller, uint8_t button) {
    switch (button) {
        case BTN_LEFT:
            buttons_controller->override_left = false;
            break;
        case BTN_MID:
            buttons_controller->override_mid = false;
            break;
        case BTN_RIGHT:
            buttons_controller->override_right = false;
            break;
    }
}

static bool isButtonPressed(uint8_t button) {
    static bool last_pressed_status[3] = {false, false, false};
    size_t idx = 0;

    switch (button) {
        case BTN_LEFT:
            idx = 0;
            break;
        case BTN_MID:
            idx = 1;
            break;
        case BTN_RIGHT:
            idx = 2;
            break;
    }

    bool pressed_status = digitalRead(button);
    if (pressed_status) {
        if (pressed_status != last_pressed_status[idx]) {
            last_pressed_status[idx] = debounceButton(button);
            return last_pressed_status[idx];
        } else {
            return false;
        }
    } else {
        last_pressed_status[idx] = false;
        return false;
    }
}

static bool debounceButton(uint8_t button) {
    static unsigned long last_read_time[3] = {0, 0, 0};
    size_t idx = 0;

    switch (button) {
        case BTN_LEFT:
            idx = 0;
            break;
        case BTN_MID:
            idx = 1;
            break;
        case BTN_RIGHT:
            idx = 2;
            break;
    }

    unsigned long read_time = millis();
    if (read_time - last_read_time[idx] > DEBOUNCE_TIME_MILLISECONDS) {
        return true;
    }

    last_read_time[idx] = read_time;
    return false;
}