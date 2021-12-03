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

#define MQ9PIN D0
float sensor_volt; 
float RS_gas; 
float ratio; 
int sensorValue;
float R0 = 0.91; //custom value here
bool gasStatus;

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

void readGasStatus(){
    sensorValue = analogRead(A0); 
    sensor_volt = ((float)sensorValue / 1024) * 5.0; 
    RS_gas = (5.0 - sensor_volt) / sensor_volt;
    ratio = RS_gas / R0;

    dRead = digitalRead(D0);

    if(dRead == HIGH){
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

    pinMode(D0, INPUT);

    delay(1000);
}

void loop() {
    readGasStatus();

    if(gasStatus){
        String measures[] = {String(gasStatus)};
        size = sizeof(measures) / sizeof(String);
        smartDevices.sendMeasures(devicesIds[0], measures, size);
    }

    delay(1000);
}
