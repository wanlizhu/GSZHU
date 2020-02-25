#pragma once

#include <functional>
#include <iomanip>
#include <unordered_map>
#include "Utils/Log.h"

namespace Wanli
{
    template<typename BaseClass, typename... Args>
    class WANLI_API Factory
    {
    public:
        using CreateReturn = std::unique_ptr<BaseClass>;
        using CreateMethod = std::function<CreateReturn(Args...)>;
        using RegistryMap = std::unordered_map<std::string, CreateMethod>;

        template<typename T>
        class Registrar : public BaseClass
        {
        public:
            static void Register(const std::string& name)
            {
                Factory::Registry()[name] = [] (Args... args) -> CreateReturn {
                    return std::make_unique<T>(std::forward<Args>(args)...);
                };
            }
        };

    public:
        Factory() = default;
        virtual ~Factory() = default;

        static CreateReturn Initialize(const std::string& name, Args&&... args)
        {
            auto it = Registry().find(name);
            if (it == Registry().end())
            {
                LOG_ERROR("Failed to create \"%s\" from Factory.\n", name.c_str());
                return nullptr;
            }
            return it->second(std::forward<Args>(args)...);
        }

        static RegistryMap& Registry()
        {
            static RegistryMap instance;
            return instance;
        }
    };
}