#pragma once

#include <lvgl.h>
#include "lv_conf.h"
#include <esp_heap_caps.h>
#include "Display_ST77916.h"
#include "Touch_CST816.h"

#define LCD_WIDTH     EXAMPLE_LCD_WIDTH
#define LCD_HEIGHT    EXAMPLE_LCD_HEIGHT
#define LVGL_BUF_LEN  (LCD_WIDTH * LCD_HEIGHT / 10)

#define EXAMPLE_LVGL_TICK_PERIOD_MS  10

#ifdef __cplusplus
extern "C" {
#endif

void Lvgl_print(const char * buf);

// LVGL v8 Display Flush Callback (uses lv_disp_drv_t instead of lv_display_t)
void Lvgl_Display_LCD(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);

// LVGL v8 Touchpad Read Callback (uses lv_indev_drv_t instead of lv_indev_t)
void Lvgl_Touchpad_Read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);

void example_increase_lvgl_tick(void *arg);

void Lvgl_Init(void);
void Lvgl_Loop(void);

#ifdef __cplusplus
}
#endif