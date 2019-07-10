#pragma once
#include <Engine/Core/Common/Delegate.h>
#include <Engine/Core/Common/NonCopyable.h>

// A value container that calls a delegate on value assignments.
// DelegateValue<Vector3f> da;
// da.Add([](Vector3f value) {
//     LOG_INFO("New Value: %s\n", value.ToString());
// });
// da = { 10.0f, -4.11f, 99.991f };

namespace ZHU
{
    template<typename T>
    class DelegatedValue : public Delegate<void(T)>
                         , public NonCopyable
    {
    public:
        template<typename... ARGS>
        DelegatedValue(ARGS... args)
            : mValue{ std::forward<ARGS>(args)... }
        {}
        virtual ~DelegatedValue() = default;

        DelegatedValue& operator=(const T& value) 
        {
            mValue = value;
            Invoke(mValue);
            return *this;
        }

        const T& Get() const { return mValue; }
        const T& operator*() const { return mValue; }
        const T* operator->() const { return &mValue; }

    private:
        T mValue;
    };
}