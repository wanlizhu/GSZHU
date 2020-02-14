#pragma once

#include "BasicTypes.h"
#include "Utilities/NonCopyable.h"

namespace Wanli
{
    enum class EModuleStage : Uint8
    {
        Never,
        Always,
        PreUpdate,
        Update,
        PostUpdate,
        Render,
    };

    template<typename _Base_>
    class DLLDECL ModuleFactory
    {
    public:
        using RegistryMap = HashMap<EModuleStage, UniquePtr<_Base_>>;
        
        template<typename T, EModuleStage _Stage_ = EModuleStage::Always>
        class DLLDECL Registrar : public _Base_
        {
        public:
            static inline T* Get() { return mModuleInstance; }
            static inline void Register() 
            {
                auto it = Registry().insert({ _Stage_, std::make_unique<T>() });
                mModuleInstance = dynamic_cast<T*>(it->second.get());
            }
            static inline void Deregister()
            {
                for (auto it = Registry().begin(); it != Registry().end();)
                {
                    if (it->second.get() == mModuleInstance)
                    {
                        it = Registry().erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }

        private:
            static inline T* mModuleInstance = nullptr;
        };

    public:
        static inline RegistryMap& Registry()
        {
            RegistryMap instance;
            return instance;
        }
    };

    class DLLDECL IModule : public NonCopyable
        , public ModuleFactory<IModule>
        , public std::enable_shared_from_this<IModule>
    {
    public:
        virtual ~IModule() = default;
        virtual void Update() = 0;
    };
}