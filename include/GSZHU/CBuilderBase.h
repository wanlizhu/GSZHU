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
        CBuilderBase(CBuilderBase const& RHS) noexcept;
        CBuilderBase& operator=(CBuilderBase const& RHS) noexcept;

        // Can be implemented inline and don't need to be exported
        CBuilderBase(CBuilderBase&& RHS) noexcept : mImpl(RHS.mImpl) { RHS.mImpl = nullptr; }
        CBuilderBase& operator=(CBuilderBase&& RHS) noexcept {
            auto Tmp = mImpl;
            mImpl = RHS.mImpl;
            RHS.mImpl = Tmp;
            return *this;
        }

    protected:
        inline T* operator->() noexcept { return mImpl; }
        inline T const* operator->() const noexcept { return mImpl; }

    protected:
        T* mImpl = nullptr;
    };
}