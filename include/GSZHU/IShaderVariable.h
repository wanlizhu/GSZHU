#pragma once

#include <GSZHU/ENUM/EShaderVariableType.h>
#include <GSZHU/IObject.h>
#include <GSZHU/IDeviceObject.h>

namespace GSZHU {
    class IShaderVariable : public IObject {
    public:
        virtual void Set(IDeviceObject* Object) = 0;
        virtual void SetArray(IDeviceObject* const* Objects, UINT FirstElement, UINT NumElements) = 0;

        virtual ESHADER_VARIABLE_TYPE GetType()const = 0;
        virtual UINT GetArraySize()const = 0;
        virtual const char* GetName()const = 0;
        virtual UINT GetIndex()const = 0;
    };
}