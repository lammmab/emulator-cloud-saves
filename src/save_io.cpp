#define MINIZ_HEADER_FILE_ONLY
#include "miniz.h"

#include <string>
#include <filesystem>
#include <iostream>

#include <fstream>
#include <unordered_map>

namespace fs = std::filesystem;

bool files_are_equal(const fs::path& f1, const fs::path& f2) {
    std::ifstream file1(f1, std::ios::binary);
    std::ifstream file2(f2, std::ios::binary);

    if (!file1 || !file2) return false;

    std::istreambuf_iterator<char> begin1(file1), end1;
    std::istreambuf_iterator<char> begin2(file2), end2;

    return std::equal(begin1, end1, begin2, end2);
}

bool are_contents_identical(const std::string& target_folder, const std::string& destination_folder) {
    fs::path target(target_folder);
    fs::path dest(destination_folder);

    if (!fs::exists(target) || !fs::exists(dest)) return false;

    std::unordered_map<std::string, fs::path> target_files;
    for (const auto& entry : fs::recursive_directory_iterator(target)) {
        if (entry.is_regular_file()) {
            auto rel = fs::relative(entry.path(), target).string();
            target_files[rel] = entry.path();
        }
    }

    for (const auto& entry : fs::recursive_directory_iterator(dest)) {
        if (entry.is_regular_file()) {
            auto rel = fs::relative(entry.path(), dest).string();

            if (!target_files.count(rel)) return false;
            if (!files_are_equal(entry.path(), target_files[rel])) return false;

            target_files.erase(rel);
        }
    }

    return target_files.empty();
}

bool folder_exists(const std::string& folder_path) {
    fs::path p(folder_path);
    return fs::exists(p) && fs::is_directory(p);
}

bool zip_folder(const std::string& folder_path, const std::string& zip_path) {
    mz_zip_archive zip;
    memset(&zip, 0, sizeof(zip));

    if (!mz_zip_writer_init_file(&zip, zip_path.c_str(), 0)) {
        std::cerr << "Failed to initialize zip: " << zip_path << "\n";
        return false;
    }

    for (const auto& entry : fs::recursive_directory_iterator(folder_path)) {
        if (entry.is_regular_file()) {
            auto rel_path = fs::relative(entry.path(), folder_path).string();
            if (!mz_zip_writer_add_file(&zip, rel_path.c_str(), entry.path().string().c_str(), nullptr, 0, MZ_DEFAULT_COMPRESSION)) {
                std::cerr << "Failed to add file to zip: " << entry.path() << "\n";
                mz_zip_writer_end(&zip);
                return false;
            }
        }
    }

    mz_zip_writer_finalize_archive(&zip);
    mz_zip_writer_end(&zip);
    return true;
}

std::string get_zip_from_folder_name(const std::filesystem::path& folder) {
    return folder.filename().string() + ".zip";
}

bool unzip_folder(const std::string& zip_path, const std::string& out_folder) {
    mz_zip_archive zip;
    memset(&zip, 0, sizeof(zip));

    if (!mz_zip_reader_init_file(&zip, zip_path.c_str(), 0)) {
        std::cerr << "Failed to open zip: " << zip_path << "\n";
        return false;
    }

    int num_files = (int)mz_zip_reader_get_num_files(&zip);
    for (int i = 0; i < num_files; i++) {
        mz_zip_archive_file_stat file_stat;
        if (!mz_zip_reader_file_stat(&zip, i, &file_stat)) continue;

        fs::path out_path = fs::path(out_folder) / file_stat.m_filename;
        fs::create_directories(out_path.parent_path());

        if (!mz_zip_reader_extract_to_file(&zip, i, out_path.string().c_str(), 0)) {
            std::cerr << "Failed to extract file: " << out_path << "\n";
            mz_zip_reader_end(&zip);
            return false;
        }
    }

    mz_zip_reader_end(&zip);
    return true;
}