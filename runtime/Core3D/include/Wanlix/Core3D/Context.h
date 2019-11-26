#pragma once

#include "Config.h"
#include "IModule.h"
#include <type_traits>
#include <vector>

namespace Wanlix
{
    class Engine;

    class Context : public std::enable_shared_from_this<Context>
    {
    public:
        using Ptr = std::shared_ptr<Context>;
        using ConstPtr = std::shared_ptr<const Context>;

        static Ptr Create();
        virtual ~Context() = default;

        bool Initialize();
        void Tick(float delta = 0.0f);

        template<typename T>
        bool RegisterModule() {
            static_assert(std::is_base_of_v<IModule, T>);
            assert(!weak_from_this().expired());

            auto mod = T::Create(weak_from_this());
            if (!mod) {
                return false;
            }
            mModules.emplace_back(std::dynamic_pointer_cast<IModule>(mod));
            return true;
        }

        template<typename T>
        std::shared_ptr<T> GetModule() {
            static_assert(std::is_base_of_v<IModule, T>);
            assert(!weak_from_this().expired());

            for (const auto& mod : mModules) {
                if (typeid(T) == typeid(*(mod.get()))) {
                    return std::static_pointer_cast<T>(mod);
                }
            }
            return nullptr;
        }

    protected:
        Context() = default;

    private:
        std::vector<std::shared_ptr<IModule>> mModules;
    };
}