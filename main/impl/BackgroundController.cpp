#include <time.h>
#include <sys/time.h>
#include <esp_attr.h>
#include "BackgroundController.h"
#include "ui_BackgroundScreen.h"
#include "ui_MusicScreen.h"
#include "sdkconfig.h"
#include <array>
#include "ui.h"
#include "lvgl.h"
#include <esp_log.h>
#include "ui_events.h"
#include <string>

static const char* TAG = "BackgroundController";

lv_indev_t* BackgroundController::GetTouchscreen(){
    lv_indev_t* indev = lv_indev_get_next(NULL);
    while(indev){
        lv_indev_type_t devType = lv_indev_get_type(indev);
        if(devType == LV_INDEV_TYPE_POINTER){
            return indev;
        }
        indev = lv_indev_get_next(indev);
    }
    return NULL;
}

uint32_t BackgroundController::GetTouchscreenIdleTime(){
    lv_indev_t* touchscreen = BackgroundController::GetTouchscreen();
    lv_display_t* display = lv_indev_get_display(touchscreen);
    if (!display) {
        display = lv_display_get_default();
    }
    return lv_disp_get_inactive_time(display);
}

bool BackgroundController::IsBackgroundActive(){
    return lv_screen_active() == ui_BackgroundScreen;
}

lv_timer_t* inactivityCheckTimer;
lv_timer_t* backgroundTimer;
lv_timer_t* updateTimeTimer;

void BackgroundController::Init(){
    inactivityCheckTimer = lv_timer_create(BackgroundController::OnTimerTick, 250, NULL);
    backgroundTimer = lv_timer_create(BackgroundController::OnBackgroundTimerTick, CONFIG_BACKGROUND_CHANGE_PERIOD, NULL);
    updateTimeTimer = lv_timer_create(BackgroundController::UpdateTime, CONFIG_BACKGROUND_TIME_UPDATE_PERIOD, NULL);
    lv_timer_pause(backgroundTimer);
    lv_timer_pause(updateTimeTimer);
}

void BackgroundController::OnTimerTick(lv_timer_t* timer){
    if(IsBackgroundActive()){
        return;
    }

    if(GetTouchscreenIdleTime() < CONFIG_AFK_MS_BEFORE_BACKGROUND){
        return;
    }

    lv_scr_load(ui_BackgroundScreen);
    lv_timer_resume(backgroundTimer);
    lv_timer_resume(updateTimeTimer);
    UpdateTime(nullptr);
}

void BackgroundController_WakeUp(lv_event_t* e){
    ESP_LOGI(TAG, "Woke up");
    lv_scr_load(ui_MusicScreen);
    lv_timer_pause(backgroundTimer);
    lv_timer_pause(updateTimeTimer);
}

static const std::array<lv_img_dsc_t, 4> backgroundImages = {
    ui_img_33876936,    // assets/Images/pexels-photo-998641.png
    ui_img_bg2_png,    // assets/bg2.png
    ui_img_bg3_png,    // assets/bg3.png
    ui_img_bg4_png     // assets/bg4.png
};
uint32_t currentImageIndex = 0;

void BackgroundController::OnBackgroundTimerTick(lv_timer_t* timer){
    currentImageIndex++;
    lv_img_set_src(ui_BackgroundImage, &backgroundImages[currentImageIndex % backgroundImages.size()]);
}

void BackgroundController::UpdateTime(lv_timer_t* timer){
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    
    char strftime_buf[64];
    if(IS_ENABLED(CONFIG_BACKGROUND_CLOCK_24HR)){
        strftime(strftime_buf, sizeof(strftime_buf), "%d-%m-%Y\n %H:%M:%S", &timeinfo);
    } else {
        strftime(strftime_buf, sizeof(strftime_buf), "%d-%m-%Y\n %-I:%M %p", &timeinfo);
    }
    lv_label_set_text(ui_Time, strftime_buf);
}