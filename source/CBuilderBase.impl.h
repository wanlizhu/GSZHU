#pragma once

#include <GSZHU/CBuilderBase.h>

namespace GSZHU {
    template<typename T>
    CBuilderBase<T>::CBuilderBase() noexcept
        : mImpl(new T) {
    }

    template<typename T>
    template<typename ... ARGS>
    CBuilderBase<T>::CBuilderBase(ARGS&& ... Args) noexcept
        : mImpl(new T(std::forward<ARGS>(Args)...)) {
    }

    template<typename T>
    CBuilderBase<T>::~CBuilderBase() noexcept {
        delete mImpl;
    }

    template<typename T>
    CBuilderBase<T>::CBuilderBase(CBuilderBase const& RHS) noexcept
        : mImpl(new T(*RHS.mImpl)) {
    }

    template<typename T>
    CBuilderBase<T>& CBuilderBase<T>::operator=(CBuilderBase<T> const& RHS) noexcept {
        *mImpl = *RHS.mImpl;
        return *this;
    }
}