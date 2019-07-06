#include "ZHUENGINE_PCH.h"
#include <ZHUENGINE/Core/ModuleManager.h>

namespace ZHU
{
    void ModuleManager::RemoveModuleFromStages(const std::type_index& type)
    {
        for (auto it = mStages.begin(); it != mStages.end(); ) {
            if (it->second == type) {
                it = mStages.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    void ModuleManager::UpdateStage(IModule::EStage stage)
    {
        for (const auto& item : mStages) {
            if (item.first != stage) {
                continue;
            }

            auto& module = mModules[item.second];
            if (module != nullptr) {
                module->Update();
            }
        }
    }
}