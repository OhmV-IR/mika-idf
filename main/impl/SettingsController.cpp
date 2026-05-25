#include "SettingsController.h"
#include "driver/gpio.h"
#include "ui_SettingsScreen.h"
#include "esp_log.h"
#include "iot_button.h"
#include "button_gpio.h"

static const char* TAG = "SettingsController";

void SettingsController::Init() {
    button_config_t btncfg{};
    btncfg.short_press_time = CONFIG_SETTINGS_BUTTON_SHORT_PRESS_TIME;
    btncfg.long_press_time = CONFIG_SETTINGS_BUTTON_LONG_PRESS_TIME;

    button_gpio_config_t gpiocfg{};
    gpiocfg.gpio_num = CONFIG_SETTINGS_BUTTON_GPIO_PIN;
    gpiocfg.active_level = 0;
    gpiocfg.enable_power_save = false;

    button_handle_t btnHandle = nullptr;
    esp_err_t e = iot_button_new_gpio_device(&btncfg, &gpiocfg, &btnHandle);
    if(e != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create button handle");
        return;
    }
    esp_err_t ecb = iot_button_register_cb(btnHandle, BUTTON_SINGLE_CLICK, nullptr, SettingsController::OnButtonPress, nullptr);
    if(ecb != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register button callback");
        return;
    }
}

void SettingsController::OnButtonPress(void*, void*) {
    ESP_LOGI(TAG, "Button pressed, navigating to settings screen");
    if(lv_screen_active() == ui_SettingsScreen) {
        ESP_LOGI(TAG, "Already on settings screen");
        return;
    }
    lv_scr_load(ui_SettingsScreen);
}

