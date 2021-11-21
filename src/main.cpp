#include <Arduino.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#include "blink_led.h"
#include <ESP8266Ping.h>


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
    blinkLed(500);

    Serial.print("Pinging host ");
    if(Ping.ping("192.168.1.45")) {
        Serial.println("Success!!");
    } else {
        Serial.println("Error :(");
    }
}
