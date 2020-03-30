#include "GIDescriptorBindingVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIDescriptorBindingVk::GIDescriptorBindingVk(SET_ID set, BINDING_ID binding)
        : mSetId(set)
        , mBindingId(binding)
    {}

    SET_ID GIDescriptorBindingVk::GetSetId() const
    {
        return mSetId;
    }

    BINDING_ID GIDescriptorBindingVk::GetBindingId() const
    {
        return mBindingId;
    }

    bool GIDescriptorBindingVk::operator==(const GIDescriptorBindingVk& rhs) const
    {
        return mSetId == rhs.mSetId && mBindingId == rhs.mBindingId;
    }

    bool GIDescriptorBindingVk::operator!=(const GIDescriptorBindingVk& rhs) const
    {
        return !(*this == rhs);
    }
}