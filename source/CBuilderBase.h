#pragma once

namespace GSZHU {
    template<typename T>
    class CBuilderBase {
    public:
        // None of these methods must be implemented inline
        template<typename... ARGS>
        explicit CBuilderBase(ARGS&&...) noexcept;
        CBuilderBase() noexcept;
        virtual ~CBuilderBase() noexcept;
        CBuilderBase(CBuilderBase const& rhs) noexcept;
        CBuilderBase& operator=(CBuilderBase const& rhs) noexcept;

        // Can be implemented inline and don't need to be exported
        CBuilderBase(CBuilderBase&& rhs) noexcept : mImpl(rhs.mImpl) { rhs.mImpl = nullptr; }
        CBuilderBase& operator=(CBuilderBase&& rhs) noexcept {
            auto tmp = mImpl;
            mImpl = rhs.mImpl;
            rhs.mImpl = tmp;
            return *this;
        }

    protected:
        inline T* operator->() noexcept { return mImpl; }
        inline T const* operator->() const noexcept { return mImpl; }

    protected:
        T* mImpl = nullptr;
    };
}