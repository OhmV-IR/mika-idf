/*****************************************************************************
  | File        :   LVGL_Driver.c
  
  | help        : 
    The provided LVGL library file must be installed first
******************************************************************************/
#include "LVGL_Driver.h"

static lv_draw_buf_t draw_buf;
static lv_color_t buf1[ LVGL_BUF_LEN ];
static lv_color_t buf2[ LVGL_BUF_LEN];
// static lv_color_t* buf1 = (lv_color_t*) heap_caps_malloc(LVGL_BUF_LEN , MALLOC_CAP_SPIRAM);
// static lv_color_t* buf2 = (lv_color_t*) heap_caps_malloc(LVGL_BUF_LEN , MALLOC_CAP_SPIRAM);
    
/*  Display flushing 
    Displays LVGL content on the LCD
    This function implements associating LVGL data to the LCD screen
*/
void Lvgl_Display_LCD( lv_display_t *disp_drv, const lv_area_t *area, uint8_t *pxMap )
{
  LCD_addWindow(area->x1, area->y1, area->x2, area->y2, ( uint16_t *)pxMap);
  lv_display_flush_ready( disp_drv );
}
/*Read the touchpad*/
void Lvgl_Touchpad_Read( lv_indev_t * indev_drv, lv_indev_data_t* data )
{
  Touch_Read_Data();
  if (touch_data.points != 0x00) {
    data->point.x = touch_data.x;
    data->point.y = touch_data.y;
    data->state = LV_INDEV_STATE_PR;
    // printf("LVGL : X=%u Y=%u points=%d\r\n",  touch_data.x , touch_data.y,touch_data.points);
  } else {
    data->state = LV_INDEV_STATE_REL;
  }
  if (touch_data.gesture != NONE ) {    
  }
  touch_data.x = 0;
  touch_data.y = 0;
  touch_data.points = 0;
  touch_data.gesture = NONE;
}
void example_increase_lvgl_tick(void *arg)
{
    /* Tell LVGL how many milliseconds has elapsed */
    lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}
void example_increase_lvgl_Loop_tick(void *arg)
{
  lv_timer_handler(); /* let the GUI do its work */
}
void Lvgl_Init(void)
{
  lv_init();
  /*Initialize the display*/
  static lv_display_t *disp = lv_display_create(LCD_WIDTH, LCD_HEIGHT);

  lv_display_set_buffers(disp, buf1, buf2, sizeof(buf1), LV_DISPLAY_RENDER_MODE_FULL);
  lv_display_set_flush_cb(disp, Lvgl_Display_LCD);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_t* indev_drv = lv_indev_create();
  lv_indev_set_type(indev_drv, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev_drv, Lvgl_Touchpad_Read);
  lv_indev_set_display(indev_drv, disp);

  /* Create simple label */
  // lv_obj_t *label = lv_label_create( lv_scr_act() );
  // lv_label_set_text( label, "Hello Ardino and LVGL!");
  // lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

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
  lv_timer_handler(); /* let the GUI do its work */
}
