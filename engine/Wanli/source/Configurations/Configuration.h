#pragma once

#include "Core/BasicTypes.h"
#include "Utilities/NonCopyable.h"

#include "Configurations/GraphicsConfig.h"
#include "Configurations/SceneConfig.h"
#include "Configurations/PhysicsConfig.h"
#include "Configurations/EventConfig.h"
#include "Configurations/InputConfig.h"

namespace Wanli
{
    class DLLDECL Configuration : public NonCopyable
    {
        friend class Engine;
    public:
        void LoadConfig(const Path& path);
        void SaveConfig(const Path& path);
        void SealUp();
        bool IsSealed() const;

        template<typename T>
        inline std::enable_if_t<std::is_base_of_v<IConfig, T>, T&>
            GetConfig()
        {
            auto it = mConfigs.find(typeid(T));
            if (it == mConfigs.end())
            {
                mConfigs[typeid(T)] = std::make_unique<T>(*this);
            }

            return *dynamic_cast<T*>(mConfigs[typeid(T)].get());
        }

    protected:
        bool mSealed = false;
        HashMap<std::type_index, UniquePtr<IConfig>> mConfigs;
    };
}