/*
  MultiSensor.ino - Example for Library SmartDevices.
  For devices: DHT11 (temperature and humidity sensor), HC-SR501 PIR (presence sensor) and MQ-9 (gas sensor).
  Created by Marcelo A. and Caio Souza, November 2, 2021.
  Released into the public domain.
*/

#include "DHT.h"
#include "ESP8266WiFi.h"
#include "WiFiClient.h"
#include "ESP8266HTTPClient.h"
#include "SmartDevices.h"

#define DHTPIN D1     // what digital pin the DHT11 is conected to
#define DHTTYPE DHT11   // setting the type as DHT11
DHT dht(DHTPIN, DHTTYPE);
int h = 0;
float t = 0;

#define PIRPIN D2
boolean presence;

#define MQ9PIN D0
float sensor_volt; 
float RS_gas; 
float ratio; 
int sensorValue
float R0 = 0.91;
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

void readDht11(){
    h = dht.readHumidity();
    t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
    }

    Serial.print("Humidity = ");
    Serial.print(h);
    Serial.print("%  ");
    Serial.print("Temperature = ");
    Serial.print(t); 
    Serial.println("C  ");
}

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

void readGasStatus(){
    sensorValue = analogRead(A0); 
    sensor_volt = ((float)sensorValue / 1024) * 5.0; 
    RS_gas = (5.0 - sensor_volt) / sensor_volt;
    ratio = RS_gas / R0;

    dRead = digitalRead(MQ9PIN);

    if(dRead == HIGH){
        gasStatus = true;
    }else {
        gasStatus = false;
    }

    Serial.print("Gas Status: " );
    Serial.println(gasStatus);
}

void setup() {
    delay(3000);
    Serial.begin(115200);
    dht.begin();
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");

    smartDevices.initConnection(devicesIds, 1);

    pinMode(PIRPIN, INPUT);
    pinMode(MQ9PIN, INPUT);

    delay(1000);
}

void loop() {

    readDht11();
    String measures[] = {String(h),String(t)};
    size = sizeof(measures) / sizeof(String);
    smartDevices.sendMeasures(devicesIds[0], measures, size);

    delay(500);

    readPresence();
    if(presence){
        String measures2[] = {String(presence)};
        size = sizeof(measures2) / sizeof(String);
        smartDevices.sendMeasures(devicesIds[0], measures2, size);
    }

    delay(500);

    readGasStatus();
    if(gasStatus){
        String measures3[] = {String(gasStatus)};
        size = sizeof(measures3) / sizeof(String);
        smartDevices.sendMeasures(devicesIds[0], measures3, size);
    }

    delay(5000);
}
