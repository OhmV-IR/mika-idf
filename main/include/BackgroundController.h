#include "lvgl.h"

namespace BackgroundController {
    lv_indev_t* GetTouchscreen();
    uint32_t GetTouchscreenIdleTime();
    void Init();
    void OnTimerTick(lv_timer_t* timer);
    bool IsBackgroundActive();
}

void BackgroundController_WakeUp();