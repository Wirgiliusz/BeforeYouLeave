#include <Arduino.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <AsyncPing.h>
#include "ESP8266IFTTTWebhook.h"
#include "ifttt_config.h"
#include "leds_controller.h"
#include "buttons_controller.h"
#include "hooks_controller.h"

void checkMissingItemsAndTriggerNotification();

bool triggered = false;
bool is_pinging = false;
bool no_response = false;

WiFiClient client;
ESP8266IFTTTWebhook ifttt(WEBHOOK_NAME, API_KEY, client);
AsyncPing ping;
struct ButtonsController buttons_controller;
struct HooksController hooks_controller;

void setup() {
    buttonsControllerInit(&buttons_controller);
    hooksControllerInit(&hooks_controller);
    ledsControllerInit();

    Serial.begin(9600);

    WiFiManager wifiManager;
    //wifiManager.resetSettings(); // Reset saved settings
    wifiManager.autoConnect("AutoConnectAP");
    Serial.println("Connected!");

    /* Callback for end of ping */
    ping.on(false, [](const AsyncPingResponse& response) {
        IPAddress addr(response.addr);

        if (response.total_recv < 1) {
            no_response = true;
        } else {
            no_response = false;
        }

        is_pinging = false;
        return true;
    });
}

void loop() {
    checkButtonsAndToggleOverride(&buttons_controller);
    checkHooksAndMarkMissingItems(&hooks_controller, &buttons_controller);
    checkMissingItemsAndLightLeds(&hooks_controller);
    checkMissingItemsAndTriggerNotification();

    Serial.print("\n");
}

void checkMissingItemsAndTriggerNotification() {
    if (hooks_controller.missing_item_left || hooks_controller.missing_item_mid || hooks_controller.missing_item_right) {
        Serial.print("Missing item detected.. ");

        if (!is_pinging) {
            Serial.print("Pinging host!\n");
            ping.begin(IP_TO_PING);
            is_pinging = true;
        } else {
            Serial.print("Pinging already in progress.\n");
        }

        if (!no_response) {
            Serial.println("Success! User at home");
        } else if (!triggered) { 
            Serial.println("User missing.. Triggering notification");
            ifttt.trigger("KEYS");
            triggered = true;
        }
    }
}