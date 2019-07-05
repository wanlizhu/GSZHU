#pragma once
#include "IModule.h"
#include <unordered_map>
#include <typeindex>
#include "Common/Log.h"

namespace ZHU
{
    class ZHU_API ModuleManager : public NonCopyable
    {
    public:
        friend class Engine;
        static ModuleManager& GetInstance();

        template<typename T>
        inline bool HasModule() const {
            const auto it = mModules.find(typeid(T));
            return it != mModules.end() && it->second != nullptr;
        }

        template<typename T>
        inline T* GetModule() const {
            auto it = mModules.find(typeid(T));
            LOG_FATAL_IF(it == mModules.end() || it->second == nullptr, 
                         "ModuleManager doesn't have requested Module");
            return static_cast<T*>(it->second.get());
        }

        template<typename T>
        inline void AddModule(IModule::EStage stage, std::unique_ptr<T>&& module) {
            LOG_ERROR_IF(module.get() == nullptr, "Try to add an empty module");
            mModules[typeid(T)] = module;
            mStages.insert(stage, typeid(T));
        }

        template<typename T>
        inline void RemoveModule() {
            RemoveModuleFromStages(typeid(T));
            mModules.erase(typeid(T));
        }

    private:
        void RemoveModuleFromStages(const std::type_index& type);
        void UpdateStage(IModule::EStage stage);

    private:
        std::unordered_map<std::type_index, std::unique_ptr<IModule>> mModules;
        std::unordered_multimap<IModule::EStage, std::type_index> mStages;
    };
}