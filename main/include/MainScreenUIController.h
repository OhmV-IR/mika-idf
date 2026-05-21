#pragma once
#include "UIController.h"
#include "ui.h"
#include "Arduino.h"

class MainScreenUIController : public UIController {
    private:
    MainScreenUIController();
    public:
    void Init();
    void Loop();
    void UpdateUI();
    void SetupEvents();
    void UpdateTimeDisplay();
    void UpdateProgressBar();
    void UpdateList();
    // Callback functions for UI events
    void UI_ModeButtonCallback(lv_event_t *e);
    void UI_PlayPauseButtonCallback(lv_event_t *e);
    void UI_NextButtonCallback(lv_event_t *e);
    void UI_PreviousButtonCallback(lv_event_t *e);
    void UI_ListCallback(lv_event_t *e);

    // New callbacks for volume and brightness controls
    void UI_VolumeArcCallback(lv_event_t *e);
    void UI_BrightnessArcCallback(lv_event_t *e);
    void TimerCallback(lv_timer_t *timer);

    static MainScreenUIController& Get();
};