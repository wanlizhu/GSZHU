#pragma once

#include "Common.h"
#include "VertexShaderAttributes.h"

namespace Wanlix
{
    struct FragmentAttribute
    {
        std::variant<std::string, SemanticName> name;

        ResourceFormat format = ResourceFormat::RGBA32Float;
        uint32_t       location = 0;
        SystemValue    systemValue = SystemValue::Undefined;

        FragmentAttribute() = default;
        FragmentAttribute(
            const std::string& name,
            const ResourceFormat& format,
            uint32_t location = 0,
            const SystemValue & systemValue = SystemValue::Undefined
        ) noexcept;
        FragmentAttribute(
            const SemanticName& semanticName,
            const ResourceFormat& format,
            uint32_t location = 0,
            const SystemValue & systemValue = SystemValue::Undefined
        ) noexcept;
    };

    struct FragmentShaderAttributes
    {
        std::vector<FragmentAttribute> outputAttribs;
    };
}