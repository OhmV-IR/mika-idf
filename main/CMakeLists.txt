file(GLOB MAIN_SOURCES "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

idf_component_register(SRCS ${MAIN_SOURCES}
                    INCLUDE_DIRS "."
		    REQUIRES Display_ST77916 esp_lcd_st77916 ESP_SR lvgl LVGL_Driver RTC_PCF85063 SD_Card TCA9554PWR Touch_CST816 ui Audio_PCM5101)

