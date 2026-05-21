#include "BackgroundController.h"
#include "ui_BackgroundScreen.h"
#include "ui_MusicScreen.h"
#include "sdkconfig.h"

lv_indev_t* BackgroundController::GetTouchscreen(){
    lv_indev_t* indev = lv_indev_get_next(NULL);
    while(indev){
        lv_indev_type_t devType = lv_indev_get_type(indev);
        if(devType == LV_INDEV_TYPE_POINTER){
            return indev;
        }
    }
    return NULL;
}

uint32_t BackgroundController::GetTouchscreenIdleTime(){
    lv_indev_t* touchscreen = BackgroundController::GetTouchscreen();
    lv_disp_t* display = lv_obj_get_disp(touchscreen->cursor);
    return lv_disp_get_inactive_time(display);
}

bool BackgroundController::IsBackgroundActive(){
    return lv_scr_act() == ui_BackgroundScreen;
}

void BackgroundController::Init(){
    lv_timer_t* backgroundTimer = lv_timer_create(BackgroundController::OnTimerTick, 250, NULL);
}

void BackgroundController::OnTimerTick(lv_timer_t* timer){
    if(IsBackgroundActive()){
        return;
    }

    if(GetTouchscreenIdleTime() < CONFIG_AFK_MS_BEFORE_BACKGROUND){
        return;
    }

    lv_scr_load(ui_BackgroundScreen);
}

void BackgroundController_WakeUp(){
    lv_scr_load(ui_MusicScreen);
}