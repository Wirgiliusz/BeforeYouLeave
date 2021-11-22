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
int no_reposnse_count = 0;
bool missing_item = false;

WiFiClient client;
ESP8266IFTTTWebhook ifttt(WEBHOOK_NAME, API_KEY, client);

void setup() {
    pinMode(D0, OUTPUT);
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
    missing_item = !digitalRead(D6);

    if (missing_item) {
        Serial.print("Pinging host: ");

        if(Ping.ping("192.168.1.45")) {
            Serial.println("Success!");
        } else {
            Serial.println("No response");
            no_reposnse_count++;
            if (no_reposnse_count >= 3 && !triggered) {
                ifttt.trigger("KEYS");
                triggered = true;
                no_reposnse_count = 0;
            }
        }
    }
    
    delay(1000);
}
