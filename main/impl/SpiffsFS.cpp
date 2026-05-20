#include "SpiffsFS.h"
#include <esp_spiffs.h>
#include <esp_log.h>
#include <string.h>

static const char* LOG_TAG = "SPIFFS_ACCESSOR";
static constexpr int MAX_SPIFFS_FILE_HANDLES = 20;

SpiffsFS& SpiffsFS::Get(){
    static SpiffsFS inst = [](){
        return SpiffsFS();
    }();
    return inst;
}

SpiffsFS::SpiffsFS(){
    ESP_LOGI(LOG_TAG, "Initializing SPIFFS");
    esp_vfs_spiffs_conf_t spiffs_conf{};
    spiffs_conf.base_path = "/spiffs";
    spiffs_conf.partition_label = "storage";
    spiffs_conf.max_files = MAX_SPIFFS_FILE_HANDLES;
    spiffs_conf.format_if_mount_failed = true;
    esp_err_t ret = esp_vfs_spiffs_register(&spiffs_conf);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(LOG_TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(LOG_TAG, "Failed to find SPIFFS partition matching label");
        } else {
            ESP_LOGE(LOG_TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }
}

FileHandle SpiffsFS::OpenFile(const char* path, const char* mode){
    int totalSize = strlen(path) + sizeof("/spiffs/");
    char* finalPath = (char*)malloc(totalSize);
    snprintf(finalPath, totalSize, "%s%s", "/spiffs/", path);
    return FileHandle(fopen(finalPath, mode));
}