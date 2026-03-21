#include "SD_Card.h"
#include <esp_vfs_fat.h>
#include <sdmmc_cmd.h>
#include <driver/sdmmc_host.h>
#include <driver/gpio.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

bool SDCard_Flag = false;
bool SDCard_Finish = false;

uint16_t SDCard_Size = 0;
uint16_t Flash_Size = 0;

void SD_D3_Dis(){
  Set_EXIO(EXIO_PIN4, Low);
  vTaskDelay(pdMS_TO_TICKS(10));
}

void SD_D3_EN(){
  Set_EXIO(EXIO_PIN4, High);
  vTaskDelay(pdMS_TO_TICKS(10));
}

sdmmc_card_t* card;

esp_err_t SD_Init() {
  // Initialize flags to a safe state
  SDCard_Flag = false;
  SDCard_Finish = false;
  
  // SD MMC
  sdmmc_slot_config_t sdslot = SDMMC_SLOT_CONFIG_DEFAULT();
  sdslot.clk = (gpio_num_t)SD_CLK_PIN;
  sdslot.cmd = (gpio_num_t)SD_CMD_PIN;
  sdslot.d0 = (gpio_num_t)SD_D0_PIN;
  sdslot.d1 = (gpio_num_t)-1;
  sdslot.d2 = (gpio_num_t)-1;
  sdslot.d3 = (gpio_num_t)-1;
  sdslot.width = 1;
  sdslot.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;
  esp_vfs_fat_sdmmc_mount_config_t mtcfg = {
	  .format_if_mount_failed = false,
	  .max_files = 5,
  };
  sdmmc_host_t host = SDMMC_HOST_DEFAULT();
  
  // Enable SD card D3 line
  SD_D3_EN();
  
  // Try to mount the SD card with a reasonable timeout
  int retry_count = 0;
  bool sd_initialized = false;
  
  while (retry_count < 3 && !sd_initialized) {

    sd_initialized = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &sdslot, &mtcfg, &card);
    if (!sd_initialized) {
      printf("SD init attempt %d failed, retrying...\r\n", retry_count + 1);
      vTaskDelay(pdMS_TO_TICKS(100)); // Wait before retry
      retry_count++;
    }
  }
  
  if (sd_initialized) {
    printf("SD card initialization successful!\r\n");
  } else {
    printf("SD card initialization failed after %d attempts!\r\n", retry_count);
    return ESP_FAIL;
  }
  
  // Set flags indicating successful initialization
  SDCard_Flag = true;
  SDCard_Finish = true;
  
  return ESP_OK;
}

bool SD_IsAvailable() {
  return SDCard_Flag;
}

void SD_End() {
  if (SDCard_Flag) {
    esp_vfs_fat_sdcard_unmount("/sdcard", card);
    sdmmc_host_deinit();
    SDCard_Flag = false;
  }
}

bool File_Search(const char* directory, const char* fileName) {
  // Check if SD is available before proceeding
  if (!SD_IsAvailable()) {
    printf("SD card not available for file search\r\n");
    return false;
  }
  DIR* dir = opendir(directory);
  if(dir == NULL){
      return false;
  }
  struct dirent* entry;
  while((entry = readdir(dir)) != NULL){
	  if(strcmp(entry->d_name, fileName) == 0){
		  closedir(dir);
		  return true;
	  }
  }
  closedir(dir);
  return false;
}

uint16_t Folder_retrieval(const char* directory, const char* fileExtension, char File_Name[][100], uint16_t maxFiles) {
  // Check if SD is available before proceeding
  if (!SD_IsAvailable()) {
    printf("SD card not available for folder retrieval\r\n");
    return 0;
  }
  DIR* dir = opendir(directory);
  if (!dir) {
    printf("Path: <%s> does not exist\r\n", directory);
    return 0;
  }
  
  uint16_t fileCount = 0;
  struct dirent* entry;
      while ((entry = readdir(dir)) != NULL && fileCount < maxFiles) {
	              if (entry->d_type == DT_DIR)
			                  continue;

		              if (strstr(entry->d_name, fileExtension) != NULL) {
				                  strncpy(File_Name[fileCount], entry->d_name, 99);
						              File_Name[fileCount][99] = '\0';
							                  fileCount++;
									          }
			          }
  
  if (fileCount > 0) {
    printf("Retrieved %d %s files\r\n", fileCount, fileExtension);
    return fileCount;
  } else {
    printf("No files with extension '%s' found in directory: %s\r\n", fileExtension, directory);
    return 0;
  }
}
