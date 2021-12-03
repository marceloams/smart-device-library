/*
  TemperatureSensor.ino - Example for Library SmartDevices.
  For device: DHT11 (temperature and humidity sensor).
  Created by Marcelo A. and Caio Souza, November 2, 2021.
  Released into the public domain.
*/

#include "DHT.h"
#include "ESP8266WiFi.h"
#include "WiFiClient.h"
#include "ESP8266HTTPClient.h"
#include "SmartDevices.h"

#define DHTPIN D1     
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);
int h = 0;
float t = 0;

const char* ssid = ""; //Enter your WIFI ssid
const char* password = ""; //Enter your WIFI password
String server_url = "";// Enter the API endpoint without '/' at the end
String devicesIds[] = {"id1", "id2"}; //Enter your device Id

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

    delay(1000);
}

void loop() {
    readDht11();

    String measures[] = {String(h),String(t)};
    size = sizeof(measures) / sizeof(String);

    smartDevices.sendMeasures(devicesIds[0], measures, size);

    delay(5000);
}
