#include "leds_controller.h"

void ledsControllerInit() {
    pinMode(LED_LEFT, OUTPUT);
    pinMode(LED_MID, OUTPUT);
    pinMode(LED_RIGHT, OUTPUT);

    turnLedOff(LED_LEFT);
    turnLedOff(LED_MID);
    turnLedOff(LED_RIGHT);
}

void turnLedOff(uint8_t led) {
    digitalWrite(led, LOW);
}