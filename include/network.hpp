#pragma once
#include <string>

bool server_ping(const std::string& url);

bool authenticate(const std::string& username, const std::string& password);
bool server_zip_download(const std::string& name, const std::string& path);
bool server_zip_upload(const std::string& name);