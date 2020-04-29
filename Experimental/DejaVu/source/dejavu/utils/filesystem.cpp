#include "utils/filesystem.h"
#include "utils/strings.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

namespace djv
{
    namespace fs = std::filesystem;

    bool is_file(const std::string& path)
    {
        const fs::path fspath(path);
        return fs::exists(fspath) && fs::is_regular_file(fspath);
    }

    bool is_directory(const std::string& path)
    {
        const fs::path fspath(path);
        return fs::exists(fspath) && fs::is_directory(fspath);
    }

    bool is_symlink(const std::string& path)
    {
        const fs::path fspath(path);
        return fs::exists(fspath) && fs::is_symlink(fspath);
    }

    bool is_equivalent(const std::string& path1, const std::string& path2)
    {
        return fs::equivalent(fs::path(path1), fs::path(path2));
    }

    bool load_file(const std::string& path, std::string& str)
    {
        std::ifstream fin(path, std::ios::in);

        if (fin.is_open()) 
        {
            std::istreambuf_iterator<char> fbegin(fin);
            std::istreambuf_iterator<char> fend;
            str = std::string(fbegin, fend);
            fin.close();
            return true;
        }

        return false;
    }

    bool load_file(const std::string& path, std::vector<uint8_t>& bytes)
    {
        std::ifstream fin(path, std::ios::binary | std::ios::in | std::ios::ate);
        
        if (fin.is_open())
        {
            bytes.resize(fin.tellg());
            fin.seekg(0, std::ios::beg);
            fin.read((char*)bytes.data(), bytes.size());
            fin.close();
            return true;
        }

        return false;
    }

    bool save_file(const std::string& path, void* data, size_t size)
    {
        std::ofstream fout(path, std::ios::binary | std::ios::out);

        if (fout.is_open())
        {
            fout.write((const char*)data, size);
            fout.close();
            return true;
        }

        return false;
    }

    bool copy_file(const std::string& from, const std::string& to, bool overwrite)
    {
        std::error_code error;
        fs::copy_options options = 
            fs::copy_options::copy_symlinks |
            (overwrite ? fs::copy_options::overwrite_existing : fs::copy_options::skip_existing);

        fs::copy_file(fs::path(from), fs::path(to), options, error);

        if (error)
        {
            std::cerr << "Error: " << error.value() << ": " << error.message() << std::endl;
            return false;
        }

        return true;
    }

    bool copy_directory(const std::string& from, const std::string& to, bool overwrite)
    {
        std::error_code error;
        fs::copy_options options =
            fs::copy_options::recursive |
            fs::copy_options::copy_symlinks |
            (overwrite ? fs::copy_options::overwrite_existing : fs::copy_options::skip_existing);

        fs::copy(fs::path(from), fs::path(to), options, error);

        if (error)
        {
            std::cerr << "Error: " << error.value() << ": " << error.message() << std::endl;
            return false;
        }

        return true;
    }

    bool remove_file(const std::string& path)
    {
        std::error_code error;
        fs::remove(fs::path(path), error);
        
        if (error)
        {
            std::cerr << "Error: " << error.value() << ": " << error.message() << std::endl;
            return false;
        }

        return true;
    }

    bool remove_directory(const std::string& path)
    {
        std::error_code error;
        fs::remove_all(fs::path(path), error);

        if (error)
        {
            std::cerr << "Error: " << error.value() << ": " << error.message() << std::endl;
            return false;
        }

        return true;
    }

    bool create_symlink(const std::string& from, const std::string& to)
    {
        std::error_code error;

        if (is_file(from))
        {
            fs::create_symlink(fs::path(from), fs::path(to), error);
        }
        else if (is_directory(from))
        {
            fs::create_directory_symlink(fs::path(from), fs::path(to), error);
        }
        else
        {
            return false;
        }

        if (error)
        {
            std::cerr << "Error: " << error.value() << ": " << error.message() << std::endl;
            return false;
        }

        return true;
    }

    bool create_directory(const std::string& path)
    {
        std::error_code error;
        fs::create_directories(fs::path(path), error);

        if (error)
        {
            std::cerr << "Error: " << error.value() << ": " << error.message() << std::endl;
            return false;
        }

        return true;
    }

    size_t file_size(const std::string& path)
    {
        std::error_code error;
        size_t size = fs::file_size(fs::path(path), error);
        return error ? 0 : size;
    }

    time_t last_write_time(const std::string& path)
    {
        std::error_code error;
        fs::file_time_type ftime = fs::last_write_time(fs::path(path), error);
        
        if (error)
        {
            std::cerr << "Error: " << error.value() << ": " << error.message() << std::endl;
            return 0;
        }

        return ftime.time_since_epoch().count();
    }

    std::string canonical(const std::string& path)
    {
        return fs::canonical(fs::path(path)).string();
    }

    std::string current_path()
    {
        return fs::current_path().string();
    }
    
    void foreach_file(const std::string& dirpath, bool recursive, const std::function<bool(const std::string&)>& func)
    {
        std::error_code error;
        fs::directory_options options = 
            fs::directory_options::follow_directory_symlink | 
            fs::directory_options::skip_permission_denied;
        
        if (recursive)
        {
            for (auto& path : fs::recursive_directory_iterator(fs::path(dirpath), options, error))
            {
                if (!func(path.path().string()))
                {
                    return;
                }
            }
        }
        else
        {
            for (auto& path : fs::directory_iterator(fs::path(dirpath), options, error))
            {
                if (!func(path.path().string()))
                {
                    return;
                }
            }
        }

        if (error)
        {
            std::cerr << "Error: " << error.value() << ": " << error.message() << std::endl;
        }
    }

    void foreach_line(const std::string& filepath, bool trim, const std::function<bool(const std::string&)>& func)
    {
        std::ifstream fin(filepath, std::ios::in);
        std::string line;

        while (std::getline(fin, line))
        {
            if (trim) {
                trim_inplace(line);
            }

            if (!func(line)) {
                break;
            }
        }

        fin.close();
    }
}