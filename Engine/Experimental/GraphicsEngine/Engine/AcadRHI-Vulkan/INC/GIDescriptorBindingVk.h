#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDescriptorBindingVk
    {
    public:
        GIDescriptorBindingVk() = default;
        GIDescriptorBindingVk(SET_ID set, BINDING_ID binding);

        SET_ID GetSetId() const;
        BINDING_ID GetBindingId() const;

        bool operator==(const GIDescriptorBindingVk& rhs) const;
        bool operator!=(const GIDescriptorBindingVk& rhs) const;

    private:
        SET_ID mSetId = 0;
        BINDING_ID mBindingId = 0;
    };
}

namespace std
{
    template<>
    struct hash<AutoCAD::Graphics::Engine::GIDescriptorBindingVk>
    {
        size_t operator()(const AutoCAD::Graphics::Engine::GIDescriptorBindingVk& rhs) const
        {
            return std::hash<uint32_t>()(rhs.GetSetId()) ^ (std::hash<uint32_t>()(rhs.GetBindingId()) << 1);
        }
    };
}