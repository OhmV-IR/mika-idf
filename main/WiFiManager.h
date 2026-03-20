#pragma once
#include "Arduino.h"
#include <WiFi.h>

// WiFi credentials - replace with your own
#define WIFI_SSID "BG-IOT"
#define WIFI_PASSWORD "QiZV@m^Iy^2gwjLve5%I"

// WiFi connection timeout in milliseconds
#define WIFI_TIMEOUT 10000

// Function declarations
bool WiFi_Init();
bool WiFi_IsConnected();
void WiFi_Reconnect();
void WiFi_PrintStatus(); 
int WiFi_GetRSSI(); 