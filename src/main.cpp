#include <Arduino.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager


void setup() {
    pinMode(D0, OUTPUT);
    Serial.begin(9600);

    WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings();

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("AutoConnectAP");

    //if you get here you have connected to the WiFi
    Serial.println("Connected!");
}

void loop() {
    digitalWrite(D0, HIGH);
    Serial.println("Idle...");
    delay(500);
    digitalWrite(D0, LOW);
    delay(500);
}