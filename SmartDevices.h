/*
  SmartDevices.h - Library for Smart Devices project.
  Created by Marcelo A. and Caio Souza, November 2, 2021.
  Released into the public domain.
*/
#ifndef SmartDevices_h
#define SmartDevices_h

#include <Arduino.h>

class WiFiClient;
class HTTPClient;

class SmartDevices {

  private: 
    WiFiClient* _client;
    HTTPClient* _http;
    bool _initStatus;
    String _apiUrl;

  public:
    SmartDevices(String apiUrl, WiFiClient& client, HTTPClient& http);
    void initConnection(String devicesIds[], int devicesSize);
    void sendMeasures(String deviceId, String measures[], int measuresSize);
};

#endif