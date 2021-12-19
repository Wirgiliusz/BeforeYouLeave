#include <Arduino.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#include <ESP8266Ping.h>

#include "blink_led.h"
#include "ESP8266IFTTTWebhook.h"
#include "ifttt_config.h"

bool triggered = false;
int no_response_count = 0;
bool missing_item = false;
bool override = false;
bool toggle_override = false;

WiFiClient client;
ESP8266IFTTTWebhook ifttt(WEBHOOK_NAME, API_KEY, client);

void setup() {
    pinMode(D0, INPUT);
    pinMode(D6, INPUT);
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
    toggle_override = digitalRead(D0);
    if (toggle_override) {
        Serial.print("Toggled override.. Current status: ");
        if (override) {
            override = false;
        } else {
            override = true;
        }
        Serial.println(override);
    }

    if (!override) {
        Serial.print("Checking hook.. Current status: ");
        missing_item = !digitalRead(D6);
        Serial.println(missing_item);
    } else {
        Serial.println("Override detected.. Ignoring hook status");
        missing_item = false;
    }

    if (missing_item) {
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
    
    delay(1000);
}
