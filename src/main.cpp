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


#define HOOK_LEFT_PIN D6
#define HOOK_MID_PIN D7
#define HOOK_RIGHT_PIN D8

void checkButtonsAndToggleOverride();
bool readButton(uint8_t button);
bool debounceButton(uint8_t button);
void checkHooksAndMarkMissingItems();
void checkMissingItemsAndLightLeds();

bool triggered = false;
bool missing_item_left = false;
bool missing_item_mid = false;
bool missing_item_right = false;



bool is_pinging = false;
bool no_response = false;

WiFiClient client;
ESP8266IFTTTWebhook ifttt(WEBHOOK_NAME, API_KEY, client);
AsyncPing ping;


void setup() {
    buttonsControllerInit();
    
    pinMode(HOOK_LEFT_PIN, INPUT);
    pinMode(HOOK_MID_PIN, INPUT);
    pinMode(HOOK_RIGHT_PIN, INPUT);

    ledsControllerInit();

    Serial.begin(9600);

    WiFiManager wifiManager;
    //wifiManager.resetSettings(); // Reset saved settings

    // Fetches ssid and pass from eeprom and tries to connect
    // If it does not connect it starts an access point with the specified name (here  "AutoConnectAP") 
    // And goes into a blocking loop awaiting configuration
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
    checkButtonsAndToggleOverride();
    checkHooksAndMarkMissingItems();
    checkMissingItemsAndLightLeds();

    if (missing_item_left || missing_item_mid || missing_item_right) {
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
    
    Serial.print("\n");
}

void checkHooksAndMarkMissingItems() {
    if (!override_left) {
        Serial.print("Checking left hook.. Current status: ");
        missing_item_left = !digitalRead(HOOK_LEFT_PIN);
        Serial.println(missing_item_left);
    } else {
        Serial.println("Override detected.. Ignoring left hook status");
        missing_item_left = false;
    }

    if (!override_mid) {
        Serial.print("Checking middle hook.. Current status: ");
        missing_item_mid = !digitalRead(HOOK_MID_PIN);
        Serial.println(missing_item_mid);
    } else {
        Serial.println("Override detected.. Ignoring middle hook status");
        missing_item_mid = false;
    }

    if (!override_right) {
        Serial.print("Checking right hook.. Current status: ");
        missing_item_right = !digitalRead(HOOK_RIGHT_PIN);
        Serial.println(missing_item_right);
    } else {
        Serial.println("Override detected.. Ignoring right hook status");
        missing_item_right = false;
    }
}

void checkMissingItemsAndLightLeds() {
    if (missing_item_left) {
        turnLedOn(LED_LEFT);
    } else {
        turnLedOff(LED_LEFT);
    }

    if (missing_item_mid) {
        turnLedOn(LED_MID);

    } else {
        turnLedOff(LED_MID);
    }

    if (missing_item_right) {
        turnLedOn(LED_RIGHT);

    } else {
        turnLedOff(LED_RIGHT);
    }
}