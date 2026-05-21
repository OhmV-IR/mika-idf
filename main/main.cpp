#include "Display_ST77916.h"
#include "Audio_PCM5101.h"
#include "RTC_PCF85063.h"
#include "LVGL_Driver.h"
#include "MIC_MSM.h"
#include "SD_Card.h"
#include "BAT_Driver.h"

#include "ui.h"  // LVGL UI initialization (e.g., from SquareLine Studio)
#include "BackgroundController.h"

// https://www.youtube.com/nishad2m8
// https://buymeacoffee.com/nishad2m8

// Flag to track SD card status
bool sd_card_available = false;

extern "C" void esp_timer_impl_update_apb_freq(uint32_t apb_ticks_per_us) {
    static uint32_t last_apb_ticks_per_us = 0;
    if (apb_ticks_per_us != last_apb_ticks_per_us) {
        printf("esp_timer_impl_update_apb_freq: %lu MHz\n", (unsigned long)apb_ticks_per_us);
        last_apb_ticks_per_us = apb_ticks_per_us;
    }
    // Note: In ESP-IDF 5.1 for ESP32-S3 with SYSTIMER implementation, 
    // esp_timer uses XTAL (40MHz) by default, so APB frequency changes 
    // do not affect its timing. This stub satisfies the Arduino component link requirement.
}

void Driver_Loop(void *parameter)
{
    // Main driver loop
    while (1)
    {
        // Regular system tasks
        PCF85063_Loop();
        BAT_Get_Volts();
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void Driver_Init()
{
    Flash_test();
    BAT_Init();
    I2C_Init();
    TCA9554PWR_Init(0x00);
    Backlight_Init();
    PCF85063_Init();
}

void setup()
{
    // Initialize serial communication first for debugging
    Serial.begin(115200);
    delay(500); // Give serial time to initialize
    
    // Initialize basic drivers
    Driver_Init();
    
    // Initialize the LCD and backlight
    LCD_Init();
    Set_Backlight(100); // Set backlight to maximum
    
    Lvgl_Init();

    // Initialize UI components (generated with SquareLine or LVGL editor)
    ui_init();
    
    // Initialize SD card with proper error handling
    Serial.println("Initializing SD card...");
    esp_err_t sd_result = SD_Init();
    if (sd_result == ESP_OK) {
        Serial.println("SD card initialized successfully");
        sd_card_available = true;
    } else {
        Serial.println("SD card initialization failed, continuing without SD card");
        sd_card_available = false;
    }
    
    // Initialize audio with proper error handling
    Serial.println("Initializing audio...");
    esp_err_t audio_result = Audio_Init();
    if (audio_result != ESP_OK) {
        Serial.println("Audio initialization failed, continuing without audio");
    }
    
    // Initialize microphone
    // MIC_Init();
    
    // Create the driver task on core 0
    // Events run on Core 0, Arduino runs on Core 1
    xTaskCreatePinnedToCore(
        Driver_Loop,
        "DriverTask",
        8192,        // Increased stack size for better stability
        NULL,
        3,
        NULL,
        0           // Core 0 for events
    );

    BackgroundController::Init();
    
    Serial.println("Setup complete");
}

int Time_Loop = 0;
void loop()
{
    // Update the LVGL UI
    Lvgl_Loop();
    
    // Small delay to prevent watchdog timeouts
    vTaskDelay(pdMS_TO_TICKS(5));
}
