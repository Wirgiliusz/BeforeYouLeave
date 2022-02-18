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

void test_override_true_after_turn_on() {
    struct ButtonsController buttons_controller;
    buttonsControllerInit(&buttons_controller);

    overrideTurnOn(&buttons_controller, BTN_LEFT);
    overrideTurnOn(&buttons_controller, BTN_MID);
    overrideTurnOn(&buttons_controller, BTN_RIGHT);

    TEST_ASSERT_TRUE(buttons_controller.override_left);
    TEST_ASSERT_TRUE(buttons_controller.override_mid);
    TEST_ASSERT_TRUE(buttons_controller.override_right);
}

void test_override_false_after_turn_off() {
    struct ButtonsController buttons_controller;
    buttonsControllerInit(&buttons_controller);

    overrideTurnOff(&buttons_controller, BTN_LEFT);
    overrideTurnOff(&buttons_controller, BTN_MID);
    overrideTurnOff(&buttons_controller, BTN_RIGHT);

    TEST_ASSERT_FALSE(buttons_controller.override_left);
    TEST_ASSERT_FALSE(buttons_controller.override_mid);
    TEST_ASSERT_FALSE(buttons_controller.override_right);
}

void test_override_mixed() {
    struct ButtonsController buttons_controller;
    buttonsControllerInit(&buttons_controller);

    overrideTurnOn(&buttons_controller, BTN_LEFT);
    overrideTurnOff(&buttons_controller, BTN_MID);
    overrideTurnOn(&buttons_controller, BTN_RIGHT);

    TEST_ASSERT_TRUE(buttons_controller.override_left);
    TEST_ASSERT_FALSE(buttons_controller.override_mid);
    TEST_ASSERT_TRUE(buttons_controller.override_right);
}

void setup() {
    delay(2000); // NOTE!!! Wait for >2 secs if board doesn't support software reset via Serial.DTR/RTS
    UNITY_BEGIN();    // Start unit testing

    RUN_TEST(test_toggles_and_overrides_false_after_initialization);
    RUN_TEST(test_override_true_after_turn_on);


    UNITY_END(); // Stop unit testing
}

void loop() {

}