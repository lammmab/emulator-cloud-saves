#pragma once
#include <string>

namespace EmulatorCloudSaves {

struct CloudConfig
{
    std::string endpoint; // Backend URL
    bool use_https = true; // Probably keep enabled, unless you have reason to disable
};

struct CloudState
{
    bool use_https;
    bool server_reachable = false;
    std::string server_url;
    std::string auth_token;
    std::string temp_folder;
};

static CloudState state;


void cloud_initialize(const CloudConfig& config);
bool cloud_login(const std::string& emulator_name);

bool upload_save_folder(const std::string& save_path);
bool download_save_folder(const std::string& save_path);
}