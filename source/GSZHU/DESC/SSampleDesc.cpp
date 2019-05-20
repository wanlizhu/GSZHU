#include <GSZHU/DESC/SSampleDesc.h>

namespace GSZHU {
    SSampleDesc::SSampleDesc() noexcept
    {}

    SSampleDesc::SSampleDesc(uint8_t _Count, uint8_t _Quality) noexcept
        : Count(_Count)
        , Quality(_Quality)
    {}

    bool SSampleDesc::operator==(const SSampleDesc& rhs) const {
        return Count == rhs.Count
            && Quality == rhs.Quality;
    }
}