#pragma once

#include <functional>
#include <iomanip>
#include <unordered_map>
#include "Utilities/Log.h"

namespace Wanli
{
    template<typename _Base_, typename... _Args_>
    class Factory
    {
    public:
        using CreateReturn = std::unique_ptr<_Base_>;
        using CreateMethod = std::function<CreateReturn(_Args_...)>;
        using RegistryMap = std::unordered_map<std::string, CreateMethod>;

        template<typename T>
        class Registrar : public _Base_
        {
        public:
            static void Register(const std::string& name)
            {
                Factory::Registry()[name] = [] (_Args_... args) -> CreateReturn {
                    return std::make_unique<T>(std::forward<_Args_>(args)...);
                };
            }
        };

    public:
        Factory() = default;
        virtual ~Factory() = default;

        static CreateReturn Create(const std::string& name, _Args_&&... args)
        {
            auto it = Registry().find(name);
            if (it == Registry().end())
            {
                LOG_ERROR("Failed to create \"%s\" from Factory.\n", name.c_str());
                return nullptr;
            }
            return it->second(std::forward<_Args_>(args)...);
        }

        static RegistryMap& Registry()
        {
            static RegistryMap instance;
            return instance;
        }
    };
}