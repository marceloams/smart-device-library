/*
  GasSensor.ino - Example for Library SmartDevices.
  For device: MQ-9 (gas sensor).
  Created by Marcelo A. and Caio Souza, November 2, 2021.
  Released into the public domain.
*/

#include "ESP8266WiFi.h"
#include "WiFiClient.h"
#include "ESP8266HTTPClient.h"
#include "SmartDevices.h"

float sensor_volt; 
int sensorValue;
bool gasStatus;

const char* ssid = ""; //Enter your WIFI ssid
const char* password = ""; //Enter your WIFI password
String server_url = "";// Enter the API endpoint without '/' at the end
String devicesIds[] = {"id1", "id2"}; //Enter your device Id

int size;

// Set up the client objet
WiFiClient client;
HTTPClient http;

SmartDevices smartDevices(server_url, client, http);

void readGasStatus(){
    sensorValue = analogRead(A0); 
    sensor_volt = ((float)sensorValue / 1024) * 5.0; 

    if(sensor_volt > 1){
        gasStatus = true;
    }else {
        gasStatus = false;
    }

    Serial.print("Gas Status: " );
    Serial.println(gasStatus);
    Serial.print("Gas voltage: " );
    Serial.println(sensor_volt);
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

    delay(1000);
}

void loop() {
    readGasStatus();

        String measures[] = {String(gasStatus)};
        size = sizeof(measures) / sizeof(String);
        smartDevices.sendMeasures(devicesIds[0], measures, size);

    delay(1000);
}
