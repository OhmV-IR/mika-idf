#include "WifiConnectController.h"
#include "ui_WifiConnectScreen.h"
#include <string>
#include <esp_wifi.h>
#include <esp_log.h>
#include <vector>
#include "ui_events.h"

static const char* TAG = "WifiConnectController";

void WifiConnectController::UpdateSSIDList() {
    std::string ssidList;
    wifi_scan_config_t cfg = {};
    cfg.ssid = NULL;
    cfg.bssid = NULL;
    cfg.channel = 0;
    cfg.show_hidden = false;
    esp_err_t ret = esp_wifi_scan_start(&cfg, true);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start Wi-Fi scan: %s", esp_err_to_name(ret));
        return;
    }
    uint16_t ap_count = 0;
    esp_wifi_scan_get_ap_num(&ap_count);
    std::vector<wifi_ap_record_t> ap_records(ap_count);
    ret = esp_wifi_scan_get_ap_records(&ap_count, ap_records.data());
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to retrieve AP records: %s", esp_err_to_name(ret));
        return;
    }
    for (int i = 0; i < ap_count; i++) {
        std::string ssid(reinterpret_cast<char*>(ap_records[i].ssid));
        
        if (ssid.empty()) {
            continue;
        }

        ESP_LOGI(TAG, "SSID: %s | RSSI: %d dBm", ssid.c_str(), ap_records[i].rssi);
        ssidList += ssid + "\n";
    }
    
    lv_roller_set_options(ui_WifiSSIDList, std::string(ssidList.begin(), ssidList.end() - 1).c_str(), LV_ROLLER_MODE_NORMAL);
    if(ap_count >= lv_roller_get_option_count(ui_WifiSSIDList)) {
        lv_roller_set_selected(ui_WifiSSIDList, ap_count - 1, LV_ANIM_OFF);
    }
}

lv_timer_t* ssidRefreshTimer;

void WifiConnectController_SSIDScreenLoaded(lv_event_t* e) {
    ssidRefreshTimer = lv_timer_create([](lv_timer_t* timer) {
        WifiConnectController::UpdateSSIDList();
    }, CONFIG_WIFI_SSID_REFRESH_INTERVAL, nullptr);
}

void WifiConnectController_SSIDCancelClicked(lv_event_t* e) {
    lv_timer_del(ssidRefreshTimer);
    ssidRefreshTimer = nullptr;
}

void WifiConnectController_SSIDSelectClicked(lv_event_t* e) {
    lv_timer_del(ssidRefreshTimer);
    ssidRefreshTimer = nullptr;
}