#include <Arduino.h>
#include <unity.h>

#include "buttons_controller.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}


void test_toggles_and_overrides_false_after_initialization() {
    struct ButtonsController buttons_controller;
    buttonsControllerInit(&buttons_controller);

    TEST_ASSERT_FALSE(buttons_controller.toggle_override_left);
    TEST_ASSERT_FALSE(buttons_controller.toggle_override_mid);
    TEST_ASSERT_FALSE(buttons_controller.toggle_override_right);

    TEST_ASSERT_FALSE(buttons_controller.override_left);
    TEST_ASSERT_FALSE(buttons_controller.override_mid);
    TEST_ASSERT_FALSE(buttons_controller.override_right);
}

void setup() {
    delay(2000); // NOTE!!! Wait for >2 secs if board doesn't support software reset via Serial.DTR/RTS
    UNITY_BEGIN();    // Start unit testing

    RUN_TEST(test_toggles_and_overrides_false_after_initialization);


    UNITY_END(); // Stop unit testing
}

void loop() {

}