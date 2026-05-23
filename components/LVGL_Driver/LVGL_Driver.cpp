/*****************************************************************************
  | File        :   LVGL_Driver.cpp
******************************************************************************/
#include "LVGL_Driver.h"

// Remove the static arrays: static lv_color_t buf1[ LVGL_BUF_LEN ];
// We will allocate these dynamically with proper hardware alignment instead.
static lv_color_t* buf1 = NULL;
static lv_color_t* buf2 = NULL;

/* Display flushing */
void Lvgl_Display_LCD( lv_display_t *disp_drv, const lv_area_t *area, uint8_t *pxMap )
{
  LCD_addWindow(area->x1, area->y1, area->x2, area->y2, ( uint16_t *)pxMap);
  lv_display_flush_ready( disp_drv );
}

/* Read the touchpad */
void Lvgl_Touchpad_Read( lv_indev_t * indev_drv, lv_indev_data_t* data )
{
  Touch_Read_Data();
  if (touch_data.points != 0x00) {
    data->point.x = touch_data.x;
    data->point.y = touch_data.y;
    data->state = LV_INDEV_STATE_PR;
  } else {
    data->state = LV_INDEV_STATE_REL;
  }
  
  touch_data.x = 0;
  touch_data.y = 0;
  touch_data.points = 0;
  touch_data.gesture = NONE;
}

void example_increase_lvgl_tick(void *arg)
{
    lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}

void Lvgl_Init(void)
{
  lv_init();

  // 1. Calculate buffer size in BYTES based on your config
  uint32_t buffer_size_bytes = LVGL_BUF_LEN * sizeof(lv_color_t);

  // 2. Safely allocate memory with 16-byte DMA alignment from internal RAM
  buf1 = (lv_color_t*) heap_caps_malloc(buffer_size_bytes, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  buf2 = (lv_color_t*) heap_caps_malloc(buffer_size_bytes, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);

  // Fallback sanity check to ensure your ESP32-S3 hasn't run out of memory space
  if (buf1 == NULL || buf2 == NULL) {
      printf("🚨 Error: LVGL DMA Buffer allocation failed! Lower your LVGL_BUF_LEN.\r\n");
      return;
  }

  /*Initialize the display*/
  static lv_display_t *disp = lv_display_create(LCD_WIDTH, LCD_HEIGHT);

  // 3. FIX: Pass the precise byte size and switch render mode to PARTIAL
  lv_display_set_buffers(disp, buf1, buf2, buffer_size_bytes, LV_DISPLAY_RENDER_MODE_PARTIAL);
  lv_display_set_flush_cb(disp, Lvgl_Display_LCD);

  /*Initialize the input device driver*/
  static lv_indev_t* indev_drv = lv_indev_create();
  lv_indev_set_type(indev_drv, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev_drv, Lvgl_Touchpad_Read);
  lv_indev_set_display(indev_drv, disp);

  const esp_timer_create_args_t lvgl_tick_timer_args = {
    .callback = &example_increase_lvgl_tick,
    .name = "lvgl_tick"
  };
  esp_timer_handle_t lvgl_tick_timer = NULL;
  esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer);
  esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000);
}

void Lvgl_Loop(void)
{
  lv_timer_handler(); 
}