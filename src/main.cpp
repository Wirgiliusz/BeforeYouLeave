#include <Arduino.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <ESP8266Ping.h>
#include "blink_led.h"
#include "ESP8266IFTTTWebhook.h"
#include "ifttt_config.h"

#define BTN_LEFT_PIN D0
#define BTN_MID_PIN D1
#define BTN_RIGHT_PIN D2

#define LED_LEFT_PIN D4
#define LED_MID_PIN D3
#define LED_RIGHT_PIN D5

#define HOOK_LEFT_PIN D6
#define HOOK_MID_PIN D7
#define HOOK_RIGHT_PIN D8

void checkButtonsAndtoggleOverride();

bool triggered = false;
int no_response_count = 0;
bool missing_item_left = false;
bool missing_item_mid = false;
bool missing_item_right = false;
bool override_left = false;
bool override_mid = false;
bool override_right = false;
bool toggle_override_left = false;
bool toggle_override_mid = false;
bool toggle_override_right = false;

WiFiClient client;
ESP8266IFTTTWebhook ifttt(WEBHOOK_NAME, API_KEY, client);

void setup() {
    pinMode(BTN_LEFT_PIN, INPUT);
    pinMode(BTN_MID_PIN, INPUT);
    pinMode(BTN_RIGHT_PIN, INPUT);
    
    pinMode(HOOK_LEFT_PIN, INPUT);
    pinMode(HOOK_MID_PIN, INPUT);
    pinMode(HOOK_RIGHT_PIN, INPUT);

    pinMode(LED_LEFT_PIN, OUTPUT);
    pinMode(LED_MID_PIN, OUTPUT);
    pinMode(LED_RIGHT_PIN, OUTPUT);
    Serial.begin(9600);

    WiFiManager wifiManager;
    //wifiManager.resetSettings(); // Reset saved settings

    // Fetches ssid and pass from eeprom and tries to connect
    // If it does not connect it starts an access point with the specified name (here  "AutoConnectAP") 
    // And goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("AutoConnectAP");

    Serial.println("Connected!");
}

void loop() {
    toggle_override_left = digitalRead(BTN_LEFT_PIN);
    toggle_override_mid = digitalRead(BTN_MID_PIN);
    toggle_override_right = digitalRead(BTN_RIGHT_PIN);
    
    checkButtonsAndtoggleOverride();

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

    if (missing_item_left) {
        digitalWrite(LED_LEFT_PIN, HIGH);
    } else {
        digitalWrite(LED_LEFT_PIN, LOW);
    }

    if (missing_item_mid) {
        digitalWrite(LED_MID_PIN, HIGH);
    } else {
        digitalWrite(LED_MID_PIN, LOW);
    }

    if (missing_item_right) {
        digitalWrite(LED_RIGHT_PIN, HIGH);
    } else {
        digitalWrite(LED_RIGHT_PIN, LOW);
    }

    if (missing_item_left || missing_item_mid || missing_item_right) {
        Serial.print("Missing item detected.. Pinging host status: ");

        if(Ping.ping("192.168.1.45")) {
            Serial.println("Success! User at home");
        } else {
            Serial.print("No response: ");
            no_response_count++;
            Serial.println(no_response_count);
            if (no_response_count >= 3 && !triggered) {  
                Serial.println("User missing.. Triggering notification");
                ifttt.trigger("KEYS");
                triggered = true;
                no_response_count = 0;
            }
        }
    }
    
    delay(500);
}

void checkButtonsAndtoggleOverride() {
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