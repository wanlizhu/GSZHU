#include "GE2/Utilities.h"
#include <memory>

namespace std
{
    template<>
    struct default_delete<GE2::CSTR> 
    {
        mutable size_t _sizeHint = 0;

        constexpr default_delete() noexcept = default;

        // construct from another default_delete
        template <class U, enable_if_t<is_convertible_v<U*, GE2::CSTR*>, int> = 0>
        default_delete(const default_delete<U>&) noexcept 
        {}

        // constructor with given array size
        constexpr default_delete(size_t size) noexcept 
            : _sizeHint(size)
        {}

        // delete an array of c-str
        void operator()(GE2::CSTR* ptr) const noexcept
        { 
            _sizeHint = (_sizeHint == 0 ? (sizeof(ptr)/sizeof(ptr[0])) 
                                        : _sizeHint);
            for (int i = 0; i < _sizeHint; i++)
                if (ptr[i] != nullptr)
                    delete[] ptr[i];
            delete[] ptr;
        }
    };

}

namespace GE2
{
    UPTR<CSTR> Unpack(const STRLIST& list)
    {
        if (list.empty() ||
            std::all_of(list.begin(), list.end(),
                        [](const STR& str) {
                            return str.empty();
                        }))
        {
            return nullptr;
        }

        size_t strArrayLen = list.size();
        char** strArray = new char*[strArrayLen];
        size_t strLen = 0;
        for (int i = 0; i < strArrayLen; i++)
        {
            strLen = list[i].size();
            strArray[i] = new char[strLen + 1]; // extra space for '\0'
            std::strncpy(strArray[i], list[i].c_str(), strLen);
            strArray[i][strLen] = '\0';
        }

        return UPTR<CSTR>((CSTR*)(strArray),
                          std::default_delete<CSTR>(strArrayLen));
    }
}