#pragma once

#include "Core/Common.h"

namespace Wanli
{
    template<typename BaseClass>
    class ConfigFactory
    {
    public:
        using RegistryMap = HashMap<std::type_index, UniquePtr<BaseClass>>;

        static inline RegistryMap& GetRegistry()
        {
            static RegistryMap registryInstance;
            return registryInstance;
        }

    public:
        template<typename T>
        class Registrar : public BaseClass
        {
        private:
            static inline T* mInstance = nullptr;

        public:
            static inline T* Get() noexcept
            {
                return mInstance;
            }

            static inline bool Register()
            {
                GetRegistry().insert({ typeid(T), std::make_unique<T>()});
                mInstance = dynamic_cast<T*>(GetRegistry()[typeid(T)].get());
                return true;
            }

            static inline bool Deregister()
            {
                for (auto it = GetRegistry().begin(); it != GetRegistry().end();)
                {
                    if (it->second.get() == mInstance)
                        it = GetRegistry().erase(it);
                    else 
                        ++it;
                }
                mInstance = nullptr;
                return true;
            }
        };
    };

    class WANLI_API IConfig : public ConfigFactory<IConfig>
    {
    public:
        IConfig() = default;
        virtual ~IConfig() = default;
    };
}