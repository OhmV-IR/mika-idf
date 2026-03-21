file(GLOB MAIN_SOURCES "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

idf_component_register(SRCS ${MAIN_SOURCES}
                    INCLUDE_DIRS "."
		    REQUIRES Audio_PCM5101 BAT_Driver Display_ST77916 esp_lcd_st77916 ESP32-audioI2S I2C_Driver lvgl LVGL_Driver MIC_MSM RTC_PCF85063 SD_Card TCA9554PWR Touch_CST816 ui)
