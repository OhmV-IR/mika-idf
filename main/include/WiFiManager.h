#pragma once
#include <WiFi.h>

// WiFi connection timeout in milliseconds
#define WIFI_TIMEOUT 10000

// Function declarations

bool WiFi_Init();
bool WiFi_IsConnected();
void WiFi_Reconnect();
void WiFi_PrintStatus(); 
int WiFi_GetRSSI(); 
