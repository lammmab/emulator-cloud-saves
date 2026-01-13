#include "cloud_interface.hpp"
#include "network.hpp"
#include "save_io.hpp"

#include <filesystem>
#include <iostream>

using namespace EmulatorCloudSaves;

void cloud_initialize(const CloudConfig& config) {
    state.server_url = config.endpoint;
    state.use_https = config.use_https;
    state.temp_folder = std::filesystem::temp_directory_path().string() + "/ecs_temp";
    std::filesystem::create_directories(state.temp_folder);
    if (server_ping(state.server_url)) {
        state.server_reachable = true;
    } else {
        std::cerr << "Warning: ECS cannot reach server at " << state.server_url
            << ". You can still use offline saves.\n";  
    }
}

bool cloud_login(const std::string& emulator_name) {
    // show pre-created login window
    // make sure it's customizable enough for emulator devs to do their own thing if they wish tho
}

bool upload_save_folder(const std::string& save_path) {
    if (!folder_exists(save_path)) {
        std::cerr << "Warning: Attempt to upload non-existent savedata folder at " << save_path
            << ". Ignoring...";
        return false;
    }
    std::filesystem::path folder(save_path);
    std::string zip_file = state.temp_folder + "/" + get_zip_from_folder_name(folder);

    if (!zip_folder(save_path, zip_file)) {
        std::cerr << "Error: Failed to compress folder " << save_path << "\n";
        return false;
    }

    // TODO: sent uploaded zip to server
    return true;
}

bool download_save_folder(const std::string& save_path) {
    // 1. connect to server and authenticate
    // 2. grab the proper save by name
    std::filesystem::path save_folder(save_path);
    std::string expected_zip_name = get_zip_from_folder_name(save_folder);
    std::string zip_path = state.temp_folder + "/" + expected_zip_name;
    
    bool success = server_zip_download(expected_zip_name,zip_path);
    if (!success) {
        std::cerr << "Failed to download save " << save_folder.filename().string() << "\n";
        return false;
    }

    if (folder_exists(save_path)) {
        std::filesystem::remove_all(save_path);
    }

    if (!unzip_folder(zip_path,save_path)) {
        std::cerr << "Error: failed to extract save " << save_path;
        return false;
    }

    return true;
}
