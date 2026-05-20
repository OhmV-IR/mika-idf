#pragma once
#include <string>
#include <optional>

class WifiConfiguration {
    static constexpr int SSID_MAX = 32;
static constexpr int PASS_MAX = 64;
    private:
    WifiConfiguration();
    public:
    char ssid[SSID_MAX];
    char pass[PASS_MAX];
    WifiConfiguration(const std::string& ssid, const std::string& passwd);
    static std::optional<WifiConfiguration> LoadFromSPIFFS();
    void SaveToSPIFFS();
};