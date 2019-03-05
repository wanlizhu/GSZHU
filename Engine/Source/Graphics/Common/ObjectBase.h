#pragma once

#include <NTGS/Graphics/IObject.h>
#include <NTGS/Core/Utilities.h>
#include <memory>

namespace NTGS {

    template<typename BaseInterface>
    class ObjectBase : public IObject {
    public:
        ObjectBase() : m_ObjectID(IDGenerator<BaseInterface>::GetID()) {}

        inline virtual SIZE GetID() const override final {
            return m_ObjectID;
        }

        inline virtual SIZE TypeID() const override final {
            return typeid(BaseInterface).hash_code();
        }

        inline virtual const char* TypeName() const override final {
            return typeid(BaseInterface).name();
        }

    private:
        SIZE m_ObjectID = static_cast<SIZE>(-1);
    };
}