#include "lvgl.h"

namespace WifiConnectController {
    void UpdateSSIDList();
}

void WifiConnectController_SSIDScreenLoaded(lv_event_t* e);

void WifiConnectController_SSIDCancelClicked(lv_event_t* e);

void WifiConnectController_SSIDSelectClicked(lv_event_t* e);