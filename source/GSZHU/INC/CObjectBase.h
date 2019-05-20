#pragma once

#include <GSZHU/BasicTools.h>
#include <memory>

namespace GSZHU {
    template<typename INTERFACE>
    class CObjectBase : public INTERFACE
                      , public std::enable_shared_from_this<CObjectBase<INTERFACE>> {
    public:
        CObjectBase(const CObjectBase&) = delete;
        CObjectBase& operator==(const CObjectBase&) = delete;
        ~CObjectBase() = default;

    private:
        CObjectBase() = default;
    };
}