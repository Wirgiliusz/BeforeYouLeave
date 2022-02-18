#include "hooks_controller.h"


void hooksControllerInit(struct HooksController *hooks_controller) {
    pinMode(HOOK_LEFT, INPUT);
    pinMode(HOOK_MID, INPUT);
    pinMode(HOOK_RIGHT, INPUT);

    hooks_controller->missing_item_left = false;    
    hooks_controller->missing_item_mid = false;    
    hooks_controller->missing_item_right = false;    
}