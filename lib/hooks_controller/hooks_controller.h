#pragma once
#include <Arduino.h>
#include "buttons_controller.h"

#define HOOK_LEFT D6
#define HOOK_MID D7
#define HOOK_RIGHT D8


struct HooksController {
    bool missing_item_left;
    bool missing_item_mid;
    bool missing_item_right;
};

void hooksControllerInit(struct HooksController *hooks_controller);
void checkHooksAndMarkMissingItems(struct HooksController *hooks_controller, struct ButtonsController *buttons_controller);