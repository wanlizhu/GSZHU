#pragma once

#include <cstdint>
#include <chrono>
#include <string>
#include <vector>
#include <functional>
#include "utils/preprocess.h"

namespace djv
{
    bool DJV_API is_file(const std::string& path);
    bool DJV_API is_directory(const std::string& path);
    bool DJV_API is_symlink(const std::string& path);
    bool DJV_API is_equivalent(const std::string& path1, const std::string& path2);
    bool DJV_API load_file(const std::string& path, std::string& str);
    bool DJV_API load_file(const std::string& path, std::vector<uint8_t>& bytes);
    bool DJV_API save_file(const std::string& path, void* data, size_t size);
    bool DJV_API copy_file(const std::string& from, const std::string& to, bool overwrite);
    bool DJV_API copy_directory(const std::string& from, const std::string& to, bool overwrite);
    bool DJV_API remove_file(const std::string& path);
    bool DJV_API remove_directory(const std::string& path);
    bool DJV_API create_symlink(const std::string& from, const std::string& to);
    bool DJV_API create_directory(const std::string& path);
    size_t DJV_API file_size(const std::string& path);
    time_t DJV_API last_write_time(const std::string& path);
    std::string DJV_API canonical(const std::string& path);
    std::string DJV_API current_path();

    void DJV_API foreach_file(const std::string& dirpath, bool recursive, const std::function<bool(const std::string&)>& func);
    void DJV_API foreach_line(const std::string& filepath, bool trim, const std::function<bool(const std::string&)>& func);
}