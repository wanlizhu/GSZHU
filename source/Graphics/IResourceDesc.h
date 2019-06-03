#pragma once

#include "IObject.h"

namespace GSZHU {
    class IResourceDesc : public IObject {
    public:
        IResourceDesc(STRING const& name)
            : mName(name)
        {}

        STRING const& GetName() const { return mName; }
        void SetName(STRING const& name) { mName = name; }

    protected:
        STRING mName;
    };
}