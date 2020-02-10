#include "HAL/FileManager.h"
#include "Utilities/String.h"
#include "physfs.h"
#include "Utilities/Log.h"
#include "Engine.h"

#ifdef CreateFile
#undef CreateFile
#endif

namespace Wanli
{
    FileManager* FileManager::Get()
    {
        FileManager instance;
        return &instance;
    }

    FileManager::FileManager()
    {
        Path appPath(Engine::Get()->GetCommandLineArgs()[0]);

        PHYSFS_init(appPath.string().c_str());
        PHYSFS_permitSymbolicLinks(1);
        if (PHYSFS_isInit() == 0)
        {
            throw std::runtime_error("Failed to initialize PHYSFS library.\n");
        }

        AddSearchPath(appPath.parent_path());
    }

    FileManager::~FileManager()
    {
        PHYSFS_deinit();
    }

    bool FileManager::Exists(const String& name) const
    {
        auto pathstr = name;
        std::replace(pathstr.begin(), pathstr.end(), '\\', '/');

        return PHYSFS_exists(pathstr.c_str()) != 0;
    }

    bool FileManager::Delete(const String& name) const
    {
        auto pathstr = name;
        std::replace(pathstr.begin(), pathstr.end(), '\\', '/');

        if (PHYSFS_isDirectory(pathstr.c_str()))
        {
            for (const auto& file : *EnumerateFiles(pathstr, false))
            {
                Delete(file);
            }
        }
        
        return PHYSFS_delete(pathstr.c_str()) != 0;
    }

    SharedPtr<File> FileManager::CreateFile(const String& name)
    {
        return std::make_shared<File>(name);
    }

    SharedPtr<StringArray> FileManager::EnumerateFiles(const String& name, bool recursive) const
    {
        auto pathstr = name;
        std::replace(pathstr.begin(), pathstr.end(), '\\', '/');
        auto rc = PHYSFS_enumerateFiles(pathstr.c_str());
        auto list = std::make_shared<StringArray>();

        for (auto it = rc; *it; it++)
        {
            if (recursive)
            {
                if (PHYSFS_isDirectory(*it))
                {
                    auto subset = EnumerateFiles(*it, recursive);
                    list->insert(list->end(), subset->begin(), subset->end());
                }
            }
            list->emplace_back(*it);
        }

        PHYSFS_freeList(rc);
        return list;
    }

    void FileManager::AddSearchPath(const Path& path)
    {
        auto pathstr = path.string();
        std::replace(pathstr.begin(), pathstr.end(), '\\', '/');

        if (PHYSFS_mount(pathstr.c_str(), nullptr, true) == 0)
        {
            LOG_WARNING("[%s] Failed to mount search path: '%s'.\n",
                        PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()),
                        pathstr.c_str());
        }
    }

    void FileManager::RemoveSearchPath(const Path& path)
    {
        auto pathstr = path.string();
        std::replace(pathstr.begin(), pathstr.end(), '\\', '/');

        if (PHYSFS_unmount(pathstr.c_str()) == 0)
        {
            LOG_WARNING("[%s] Failed to unmount search path: '%s'.\n",
                        PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()),
                        pathstr.c_str());
        }
    }

    void FileManager::ClearSearchPaths()
    {
        for (const auto& path : *GetSearchPaths())
        {
            if (PHYSFS_unmount(path.c_str()) == 0)
            {
                LOG_WARNING("[%s] Failed to unmount search path: '%s'.\n",
                            PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()),
                            path.c_str());
            }
        }
    }

    SharedPtr<StringArray> FileManager::GetSearchPaths() const
    {
        auto list = std::make_shared<StringArray>();
        auto rc = PHYSFS_getSearchPath();
        for (auto it = rc; *it; it++)
        {
            list->push_back(*it);
        }
        return list;
    }
}