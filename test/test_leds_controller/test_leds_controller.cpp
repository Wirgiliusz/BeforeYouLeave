#include <Arduino.h>
#include <unity.h>

#include "leds_controller.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}


void test_leds_off_after_initialization() {
    ledsControllerInit();
    TEST_ASSERT_EQUAL(LOW, digitalRead(LED_LEFT));
    TEST_ASSERT_EQUAL(LOW, digitalRead(LED_MID));
    TEST_ASSERT_EQUAL(LOW, digitalRead(LED_RIGHT));
}


void setup() {
    delay(2000); // NOTE!!! Wait for >2 secs if board doesn't support software reset via Serial.DTR/RTS
    UNITY_BEGIN();    // Start unit testing

    RUN_TEST(test_leds_off_after_initialization);
    delay(100);

    UNITY_END(); // Stop unit testing
}

void loop() {

}