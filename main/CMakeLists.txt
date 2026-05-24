file(GLOB_RECURSE MAIN_SOURCES "${CMAKE_CURRENT_LIST_DIR}/impl/*.cpp")
file(GLOB_RECURSE UI_SOURCES "${CMAKE_CURRENT_LIST_DIR}/ui/*.c")
list(APPEND MAIN_SOURCES "${CMAKE_CURRENT_LIST_DIR}/main.cpp")

idf_component_register(SRCS ${MAIN_SOURCES} ${UI_SOURCES}
                    INCLUDE_DIRS "include" "ui" "ui/screens"
		    REQUIRES Audio_PCM5101 BAT_Driver Display_ST77916 esp_lcd_st77916 ESP32-audioI2S I2C_Driver lvgl LVGL_Driver MIC_MSM RTC_PCF85063 SD_Card TCA9554PWR Touch_CST816 spiffs)


spiffs_create_partition_image(storage resources FLASH_IN_PROJECT)
