#pragma once

#include <algorithm>
#include <vector>
#include <utility>
#include <unordered_map>
#include <functional>
#include <memory>
#include <type_traits>

namespace Wanlix
{
    template<typename RT, typename... ARGS>
    class Signal;

    template<typename RT, typename... ARGS>
    class Signal<RT(ARGS...)>
    {
    public:
        // bind free function or static method
        template<auto _FuncPtr_>
        inline uint64_t Connect() {
            uint64_t id = mLastId++;
            mConnections[id] = [](ARGS&& ... args) -> RT {
                return (*_FuncPtr_)(std::forward<ARGS>(args)...);
            };
            return id;
        }

        // bind non-static method
        template<auto _MethodPtr_, typename _Class_>
        inline uint64_t Connect(_Class_* inst) {
            uint64_t id = mLastId++;
            mConnections[id] = [](ARGS&& ... args) -> RT {
                return (static_cast<_Class_*>(inst)->*_MethodPtr_)(std::forward<ARGS>(args)...);
            };
            return id;
        }

        // bind functor
        template<typename _Functor_>
        inline uint64_t Connect(_Functor_* functor) {
            uint64_t id = mLastId++;
            mConnections[id] = [](ARGS&& ... args) -> RT {
                return static_cast<_Functor_*>(functor)->operator()(std::forward<ARGS>(args)...);
            };
            return id;
        }

        inline void Disconnect(uint64_t id) {
            auto it = mConnections.find(id);
            if (it != mConnections.end()) {
                it.erase(it);
            }
        }

        inline bool IsConnected(uint64_t id) const {
            return mConnections.find(id) != mConnections.end();
        }

        inline void Clear() {
            mConnections.clear();
        }

        template<typename... Uref>
        inline void operator()(Uref&& ... args) {
            for (auto& [id, func] : mConnections) {
                func(std::forward<Uref&&>(args)...);
            }
        }


    private:
        static uint64_t mLastId;
        std::unordered_map<uint64_t, std::function<RT(ARGS...)>> mConnections;
    };

    template<typename RT, typename... ARGS>
    uint64_t Signal<RT(ARGS...)>::mLastId = 0;
}