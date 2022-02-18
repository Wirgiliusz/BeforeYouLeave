#pragma once
#include <Arduino.h>
#include "hooks_controller.h"

#define LED_LEFT D4
#define LED_MID D3
#define LED_RIGHT D5

void ledsControllerInit();
void checkMissingItemsAndLightLeds(struct HooksController *hooks_controller);
void turnLedOff(uint8_t led);
void turnLedOn(uint8_t led);
