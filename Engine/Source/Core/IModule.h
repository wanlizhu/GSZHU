#pragma once

#include "Core/Common.h"
#include "Utils/NonCopyable.h"
#include "Configuration/IConfig.h"

namespace Wanli
{
    enum class EModuleStage
    {
        Always,
        PreUpdate,
        Update,
        PostUpdate,
        Render,
    };

    template<typename BaseClass>
    class WANLI_API ModuleFactory
    {
    public:
        using RegistryType = std::unordered_multimap<EModuleStage, std::shared_ptr<BaseClass>>;

        static inline RegistryType& GetRegistry() 
        {
            static RegistryType registryInstance;
            return registryInstance;
        }

    public:
        template<typename T, typename Config, EModuleStage Stage>
        class WANLI_API Registrar : public BaseClass
        {
        private:
            static inline T* mInstance = nullptr;

        public:
            static inline T* Get() { return mInstance; }
            static inline bool Register() 
            {
                if constexpr (std::is_base_of_v<IConfig, Config>)
                {
                    assert(Config::Get()->IsDependencyResolved());
                }

                T* module = new T();
                auto it = GetRegistry().insert(std::make_pair(Stage, std::shared_ptr<BaseClass>(module)));
                mInstance = dynamic_cast<T*>(it->second.get());
                
                if constexpr (std::is_base_of_v<IConfig, Config>)
                {
                    Config::Get()->OnCompleted();
                }

                return true;
            }
            static inline bool Deregister() 
            {
                for (auto it = GetRegistry().begin(); it != GetRegistry().end();)
                {
                    if (it->second.get() == mInstance)
                    {
                        it = GetRegistry().erase(it);
                        continue;
                    }
                    ++it;
                }
                mInstance = nullptr;
                return true;
            }
        };
    };

    class WANLI_API IModule : public NonCopyable
        , public ModuleFactory<IModule>
    {
    public:
        IModule() = default;
        virtual ~IModule() = default;

        virtual void Update() = 0;
    };
}