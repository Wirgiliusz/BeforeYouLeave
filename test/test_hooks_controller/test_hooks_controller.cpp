#include <Arduino.h>
#include <unity.h>

#include "hooks_controller.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}


void test_missing_items_false_after_initialization() {
    struct HooksController hooks_controller;
    hooksControllerInit(&hooks_controller);

    TEST_ASSERT_FALSE(hooks_controller.missing_item_left);
    TEST_ASSERT_FALSE(hooks_controller.missing_item_mid);
    TEST_ASSERT_FALSE(hooks_controller.missing_item_right);
}

void setup() {
    delay(2000); // NOTE!!! Wait for >2 secs if board doesn't support software reset via Serial.DTR/RTS
    UNITY_BEGIN();    // Start unit testing

    RUN_TEST(test_missing_items_false_after_initialization);

    UNITY_END(); // Stop unit testing
}

void loop() {

}