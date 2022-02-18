#pragma once
#include <Arduino.h>

#define LED_LEFT D4
#define LED_MID D3
#define LED_RIGHT D5

void ledsControllerInit();
void turnLedOff(uint8_t led);
void turnLedOn(uint8_t led);