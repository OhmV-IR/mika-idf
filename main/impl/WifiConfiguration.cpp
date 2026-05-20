#include <WifiConfiguration.h>
#include <SpiffsFS.h>
#include <string.h>

static constexpr const char* WIFI_SSID_PATH = "/spiffs/wifi/ssid.txt";
static constexpr const char* WIFI_PASSWD_PATH = "/spiffs/wifi/pass.txt";
static constexpr const char* LOG_TAG = "WifiConfiguration";


WifiConfiguration::WifiConfiguration(const std::string& cfgSsid, const std::string& passwd){
    strcpy(ssid, cfgSsid.c_str());
    strcpy(pass, passwd.c_str());
}

WifiConfiguration::WifiConfiguration(){

}

std::optional<WifiConfiguration> WifiConfiguration::LoadFromSPIFFS(){
    FileHandle ssidFile = SpiffsFS::Get().OpenFile(WIFI_SSID_PATH, "r");
    FileHandle passFile = SpiffsFS::Get().OpenFile(WIFI_PASSWD_PATH, "r");
    if(!ssidFile || !passFile){
        return std::nullopt;
    }
    WifiConfiguration cfg;
    fread(cfg.ssid, sizeof(char), SSID_MAX, ssidFile);
    fread(cfg.pass, sizeof(char), PASS_MAX, ssidFile);
    return cfg;
}

void WifiConfiguration::SaveToSPIFFS(){
    FileHandle ssidFile = SpiffsFS::Get().OpenFile(WIFI_SSID_PATH, "w");
    FileHandle passFile = SpiffsFS::Get().OpenFile(WIFI_PASSWD_PATH, "w");

    fwrite(ssid, sizeof(char), sizeof(ssid), ssidFile);
    fwrite(pass, sizeof(char), sizeof(pass), passFile);
}