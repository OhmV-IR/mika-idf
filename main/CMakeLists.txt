file(GLOB MAIN_SOURCES "${CMAKE_CURRENT_LIST_DIR}/impl/*.cpp")
list(APPEND MAIN_SOURCES "${CMAKE_CURRENT_LIST_DIR}/main.cpp")

idf_component_register(SRCS ${MAIN_SOURCES}
                    INCLUDE_DIRS "include"
		    REQUIRES Audio_PCM5101 BAT_Driver Display_ST77916 esp_lcd_st77916 ESP32-audioI2S I2C_Driver lvgl LVGL_Driver MIC_MSM RTC_PCF85063 SD_Card TCA9554PWR Touch_CST816 ui spiffs)


spiffs_create_partition_image(storage resources FLASH_IN_PROJECT)