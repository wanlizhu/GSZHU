#pragma once

#include <memory>

namespace GSZHU {
    template<typename T>
    class CManagedAPI : public std::enable_shared_from_this<T> {
    public:
        // Disable copy and assignment
        CManagedAPI(CManagedAPI const&) = delete;
        CManagedAPI(CManagedAPI&&) = delete;
        CManagedAPI& operator=(CManagedAPI const&) = delete;
        CManagedAPI& operator=(CManagedAPI&&) = delete;

    protected:
        CManagedAPI() noexcept = default;
        virtual ~CManagedAPI() = default;
    };
}