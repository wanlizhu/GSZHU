#pragma once

#include <cstdint>
#include <chrono>
#include <string>
#include <vector>
#include <functional>
#include "dejavu/core/exceptions.h"

namespace djv
{
    bool is_file(const std::string& path);
    bool is_directory(const std::string& path);
    bool is_symlink(const std::string& path);
    bool is_equivalent(const std::string& path1, const std::string& path2);
    bool load_file(const std::string& path, std::string& str);
    bool load_file(const std::string& path, std::vector<uint8_t>& bytes);
    bool save_file(const std::string& path, void* data, size_t size);
    bool copy_file(const std::string& from, const std::string& to, bool overwrite);
    bool copy_directory(const std::string& from, const std::string& to, bool overwrite);
    bool remove_file(const std::string& path);
    bool remove_directory(const std::string& path);
    bool create_symlink(const std::string& from, const std::string& to);
    bool create_directory(const std::string& path);
    size_t file_size(const std::string& path);
    time_t last_write_time(const std::string& path);
    std::string canonical(const std::string& path);
    std::string current_path();

    void foreach_file(const std::string& dirpath, bool recursive, const std::function<bool(const std::string&)>& func);
    void foreach_line(const std::string& filepath, bool trim, const std::function<bool(const std::string&)>& func);
}