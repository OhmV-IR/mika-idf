#pragma once
#include "AudioPlayer.h"

class UIController {
    public:
    virtual void Init() = 0;
    virtual void Loop() = 0;
    virtual void UpdateUI() = 0;
    virtual void SetupEvents() = 0;
};