#include "leds_controller.h"

void ledsControllerInit() {
    pinMode(LED_LEFT, OUTPUT);
    pinMode(LED_MID, OUTPUT);
    pinMode(LED_RIGHT, OUTPUT);

    turnLedOff(LED_LEFT);
    turnLedOff(LED_MID);
    turnLedOff(LED_RIGHT);
}

void checkMissingItemsAndLightLeds(struct HooksController *hooks_controller) {
    if (hooks_controller->missing_item_left) {
        turnLedOn(LED_LEFT);
    } else {
        turnLedOff(LED_LEFT);
    }

    if (hooks_controller->missing_item_mid) {
        turnLedOn(LED_MID);

    } else {
        turnLedOff(LED_MID);
    }

    if (hooks_controller->missing_item_right) {
        turnLedOn(LED_RIGHT);

    } else {
        turnLedOff(LED_RIGHT);
    }
}

void turnLedOff(uint8_t led) {
    digitalWrite(led, LOW);
}

void turnLedOn(uint8_t led) {
    digitalWrite(led, HIGH);
}
