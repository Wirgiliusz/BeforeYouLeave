#include "blink_led.h"



uint8_t blinkLed(uint16_t interval) {
    static uint8_t blink_count = 0;
    digitalWrite(D0, HIGH);
    delay(500);
    digitalWrite(D0, LOW);
    delay(500);
    blink_count++;

    return blink_count;
}
