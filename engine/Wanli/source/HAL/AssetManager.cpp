#include "HAL/AssetManager.h"
#include "Utilities/String.h"
#include "physfs.h"
#include "Utilities/Log.h"
#include "Engine.h"

#ifdef CreateFile
#undef CreateFile
#endif

namespace Wanli
{
    AssetManager* AssetManager::Get()
    {
        AssetManager instance;
        return &instance;
    }

    AssetManager::AssetManager()
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

    AssetManager::~AssetManager()
    {
        PHYSFS_deinit();
    }

    bool AssetManager::Exists(const String& name) const
    {
        auto pathstr = name;
        std::replace(pathstr.begin(), pathstr.end(), '\\', '/');

        return PHYSFS_exists(pathstr.c_str()) != 0;
    }

    bool AssetManager::Delete(const String& name) const
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

    SharedPtr<AssetFile> AssetManager::CreateFile(const String& name)
    {
        return std::make_shared<AssetFile>(name);
    }

    SharedPtr<StringArray> AssetManager::EnumerateFiles(const String& name, bool recursive) const
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

    void AssetManager::AddSearchPath(const Path& path)
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

    void AssetManager::RemoveSearchPath(const Path& path)
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

    void AssetManager::ClearSearchPaths()
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

    SharedPtr<StringArray> AssetManager::GetSearchPaths() const
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