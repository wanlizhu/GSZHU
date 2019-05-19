#pragma once

#include <GSZHU/ENUM/EShaderVariableType.h>
#include <GSZHU/IObject.h>
#include <GSZHU/IDeviceObject.h>

namespace GSZHU {
    class IShaderVariable : public IObject {
    public:
        virtual void Set(IDeviceObject* Object) = 0;
        virtual void SetArray(IDeviceObject* const* Objects, uint32_t FirstElement, uint32_t NumElements) = 0;

        virtual ESHADER_VARIABLE_TYPE GetType()const = 0;
        virtual uint32_t GetArraySize()const = 0;
        virtual const char* GetName()const = 0;
        virtual uint32_t GetIndex()const = 0;
    };
}