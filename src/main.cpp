#include <Arduino.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <AsyncPing.h>
#include "ESP8266IFTTTWebhook.h"
#include "ifttt_config.h"
#include "leds_controller.h"

#define DEBOUNCE_TIME_MILLISECONDS 20

#define BTN_LEFT_PIN D2
#define BTN_MID_PIN D1
#define BTN_RIGHT_PIN D0

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
bool override_left = false;
bool override_mid = false;
bool override_right = false;
bool toggle_override_left = false;
bool toggle_override_mid = false;
bool toggle_override_right = false;

bool is_pinging = false;
bool no_response = false;

WiFiClient client;
ESP8266IFTTTWebhook ifttt(WEBHOOK_NAME, API_KEY, client);
AsyncPing ping;


void setup() {
    pinMode(BTN_LEFT_PIN, INPUT);
    pinMode(BTN_MID_PIN, INPUT);
    pinMode(BTN_RIGHT_PIN, INPUT);
    
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

void checkButtonsAndToggleOverride() {
    toggle_override_left = readButton(BTN_LEFT_PIN);
    toggle_override_mid = readButton(BTN_MID_PIN);
    toggle_override_right = readButton(BTN_RIGHT_PIN);
    
    if (toggle_override_left) {
        Serial.print("Toggled override for left hook.. Current status: ");
        if (override_left) {
            override_left = false;
        } else {
            override_left = true;
        }
        Serial.println(override_left);
    }

    if (toggle_override_mid) {
        Serial.print("Toggled override for middle hook.. Current status: ");
        if (override_mid) {
            override_mid = false;
        } else {
            override_mid = true;
        }
        Serial.println(override_mid);
    }

    if (toggle_override_right) {
        Serial.print("Toggled override for right hook.. Current status: ");
        if (override_right) {
            override_right = false;
        } else {
            override_right = true;
        }
        Serial.println(override_right);
    }
}

bool readButton(uint8_t button) {
    static bool last_pressed_status[3] = {false, false, false};
    size_t idx = 0;

    switch (button) {
        case BTN_LEFT_PIN:
            idx = 0;
            break;
        case BTN_MID_PIN:
            idx = 1;
            break;
        case BTN_RIGHT_PIN:
            idx = 2;
            break;
    }

    bool pressed_status = digitalRead(button);
    if (pressed_status) {
        if (pressed_status != last_pressed_status[idx]) {
            last_pressed_status[idx] = debounceButton(button);
            return last_pressed_status[idx];
        } else {
            return false;
        }
    } else {
        last_pressed_status[idx] = false;
        return false;
    }
}

bool debounceButton(uint8_t button) {
    static unsigned long last_read_time[3] = {0, 0, 0};
    size_t idx = 0;

    switch (button) {
        case BTN_LEFT_PIN:
            idx = 0;
            break;
        case BTN_MID_PIN:
            idx = 1;
            break;
        case BTN_RIGHT_PIN:
            idx = 2;
            break;
    }

    unsigned long read_time = millis();
    if (read_time - last_read_time[idx] > DEBOUNCE_TIME_MILLISECONDS) {
        return true;
    }

    last_read_time[idx] = read_time;
    return false;
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