#include "hooks_controller.h"


void hooksControllerInit(struct HooksController *hooks_controller) {
    pinMode(HOOK_LEFT, INPUT);
    pinMode(HOOK_MID, INPUT);
    pinMode(HOOK_RIGHT, INPUT);

    hooks_controller->missing_item_left = false;    
    hooks_controller->missing_item_mid = false;    
    hooks_controller->missing_item_right = false;    
}

void checkHooksAndMarkMissingItems(struct HooksController *hooks_controller, struct ButtonsController *buttons_controller) {
    if (!buttons_controller->override_left) {
        Serial.print("Checking left hook.. Current status: ");
        hooks_controller->missing_item_left = !digitalRead(HOOK_LEFT);
        Serial.println(hooks_controller->missing_item_left);
    } else {
        Serial.println("Override detected.. Ignoring left hook status");
        hooks_controller->missing_item_left = false;
    }

    if (!buttons_controller->override_mid) {
        Serial.print("Checking middle hook.. Current status: ");
        hooks_controller->missing_item_mid = !digitalRead(HOOK_MID);
        Serial.println(hooks_controller->missing_item_mid);
    } else {
        Serial.println("Override detected.. Ignoring middle hook status");
        hooks_controller->missing_item_mid = false;
    }

    if (!buttons_controller->override_right) {
        Serial.print("Checking right hook.. Current status: ");
        hooks_controller->missing_item_right = !digitalRead(HOOK_RIGHT);
        Serial.println(hooks_controller->missing_item_right);
    } else {
        Serial.println("Override detected.. Ignoring right hook status");
        hooks_controller->missing_item_right = false;
    }
}