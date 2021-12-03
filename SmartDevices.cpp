/*
  SmartDevices.cpp - Library for Smart Devices project.
  Created by Marcelo A. and Caio Souza, November 2, 2021.
  Released into the public domain.
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <Arduino.h>
#include <SmartDevices.h>

SmartDevices::SmartDevices(String apiUrl, WiFiClient& client, HTTPClient& http){
    this->_apiUrl = apiUrl;
    this->_client = &client;
    this->_http = &http;
}

void SmartDevices::initConnection(String devicesIds[], int devicesSize){

    String server_url = _apiUrl + "/init";

    String json_str = "{\"devicesIds\":[";

    for(int i=0; i<devicesSize; i++){
        if(i>0){
            json_str = json_str + "\"" + devicesIds[i] + "\",";
        } else {
            json_str = json_str + "\"" + devicesIds[i] + "\"";
        }
    }

    json_str = json_str + "]}";

    _http->begin(*_client, server_url);
    _http->addHeader("Content-Type", "application/json");
    int httpCode = _http->POST(json_str);

    if (httpCode > 0) {
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = _http->getString();
        Serial.print("Response: ");
        Serial.println(payload);
        _initStatus = true;
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", _http->errorToString(httpCode).c_str());
    }
}

void SmartDevices::sendMeasures(String deviceId, String measures[], int measuresSize){
    if(_initStatus){
        String server_url = _apiUrl + "/send-measures";

        String json_str = "{\"deviceId\":\"" + deviceId + "\", \"measures\":[";

       for(int i=0; i<measuresSize; i++){
            if(measuresSize>1 && i<(measuresSize-1)){
                json_str = json_str + "\"" + measures[i] + "\",";
            } else {
                json_str = json_str + "\"" +  measures[i] + "\"";
            }
        }

        json_str = json_str + "]}";

        Serial.println(json_str);

        _http->begin(*_client, server_url);
        _http->addHeader("Content-Type", "application/json");
        int httpCode = _http->POST(json_str);

        if (httpCode > 0) {
            Serial.printf("[HTTP] POST... code: %d\n", httpCode);
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                String payload = _http->getString();
                Serial.print("Response: ");
                Serial.println(payload);
            }
        } else {
            Serial.printf("[HTTP] POST... failed, error: %s\n", _http->errorToString(httpCode).c_str());
        }
    } else {
        Serial.println("Device not loaded! Device Id incorrect or something went wrong!");
    }
}