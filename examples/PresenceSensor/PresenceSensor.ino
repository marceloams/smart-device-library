/*
  PresenceSensor.ino - Example for Library SmartDevices.
  For device: HC-SR501 PIR (presence sensor).
  Created by Marcelo A. and Caio Souza, November 2, 2021.
  Released into the public domain.
*/

#include "ESP8266WiFi.h"
#include "WiFiClient.h"
#include "ESP8266HTTPClient.h"
#include "SmartDevices.h"

#define PIRPIN D2
boolean presence;

const char* ssid = ""; //Enter your WIFI ssid
const char* password = ""; //Enter your WIFI password
String server_url = "";// Enter the API endpoint without '/' at the end
String devicesIds[] = {"id1", "id2"}; //Enter your device Id

int dRead;
int size;

// Set up the client objet
WiFiClient client;
HTTPClient http;

SmartDevices smartDevices(server_url, client, http);

void readPresence(){
    dRead = digitalRead(PIRPIN);
    if(dRead == HIGH){
        presence = true;
    }else {
        presence = false;
    }

    Serial.print("Presence: " );
    Serial.println(presence);
}

void setup() {
    delay(3000);
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");

    smartDevices.initConnection(devicesIds, 1);

    pinMode(PIRPIN, INPUT);

    delay(1000);
}

void loop() {
    readPresence();

        String measures[] = {String(presence)};
        size = sizeof(measures) / sizeof(String);
        smartDevices.sendMeasures(devicesIds[0], measures, size);

    delay(1000);
}
