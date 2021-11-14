#include <Arduino.h>
#include <unity.h>

#include "blink_led.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}


void test_led_state_high(void) {
    digitalWrite(LED_PIN, HIGH);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(LED_PIN));
}

void test_led_state_low(void) {
    digitalWrite(LED_PIN, LOW);
    TEST_ASSERT_EQUAL(LOW, digitalRead(LED_PIN));
}

void test_led_blink(void) {
    TEST_ASSERT_EQUAL_UINT8(1, blinkLed(500));
    TEST_ASSERT_EQUAL_UINT8(2, blinkLed(250));
    TEST_ASSERT_EQUAL_UINT8(3, blinkLed(100));
}

void setup() {
    delay(2000); // NOTE!!! Wait for >2 secs if board doesn't support software reset via Serial.DTR/RTS
    UNITY_BEGIN();    // Start unit testing

    pinMode(LED_PIN, OUTPUT);

    RUN_TEST(test_led_state_high);
    delay(100);
    RUN_TEST(test_led_state_low);
    delay(100);
    RUN_TEST(test_led_blink);
    delay(100);

    UNITY_END(); // Stop unit testing
}

void loop() {

}