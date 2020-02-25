#pragma once

#include "Core/Common.h"
#include "Utils/NonCopyable.h"

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
    class ModuleFactory
    {
    public:
        using RegistryType = std::unordered_multimap<EModuleStage, std::unique_ptr<BaseClass>>;

        static inline RegistryType& GetRegistry() 
        {
            static RegistryType registryInstance;
            return registryInstance;
        }

    public:
        template<typename T, EModuleStage Stage>
        class Registrar : public BaseClass
        {
        private:
            static inline T* mInstance = nullptr;

        public:
            static inline T* Get() { return mInstance; }
            static inline bool Register() 
            {
                auto it = GetRegistry().insert({ Stage, std::make_unique<T>() });
                mInstance = dynamic_cast<T*>(it->second.get());
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
        virtual ~IModule() = default;

        virtual void Initialize() = 0;
        virtual void Update() = 0;
        virtual void Destroy() = 0;
    };
}