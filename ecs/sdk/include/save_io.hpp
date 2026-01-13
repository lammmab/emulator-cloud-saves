#pragma once
#include <string>

bool zip_folder(const std::string& folder_path, const std::string& zip_path);
bool unzip_folder(const std::string& zip_path, const std::string& out_folder);
bool folder_exists(const std::string& folder_path);
bool are_contents_identical(const std::string& target_folder, const std::string& destination_folder);
std::string get_zip_from_folder_name(const std::filesystem::path& folder);