#include "BackgroundController.h"
#include "ui_BackgroundScreen.h"
#include "ui_MusicScreen.h"
#include "sdkconfig.h"
#include <array>
#include "ui.h"
#include "lvgl.h"

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

void BackgroundController::Init(){
    inactivityCheckTimer = lv_timer_create(BackgroundController::OnTimerTick, 250, NULL);
    backgroundTimer = lv_timer_create(BackgroundController::OnBackgroundTimerTick, CONFIG_BACKGROUND_CHANGE_PERIOD, NULL);
    lv_timer_pause(backgroundTimer);
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
}

void BackgroundController_WakeUp(){
    lv_scr_load(ui_MusicScreen);
    lv_timer_pause(backgroundTimer);
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