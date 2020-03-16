#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class SPIRVShaderVariable
    {
    public:
        virtual const std::string& GetName() const = 0;
        virtual uint32_t GetDescriptorSetID() const = 0;
        virtual uint32_t GetBindingID() const = 0;
        virtual uint32_t GetLocation() const = 0; // Only for in/out variables
        virtual uint32_t GetOffset() const = 0;
        virtual uint32_t GetSize() const = 0;
        virtual VkDescriptorType GetDescriptorType() const = 0;
        virtual VkPipelineStageFlagBits GetVisibleStages() const = 0;
    };

    class SPIRVAttribute : public SPIRVShaderVariable
    {
    public:
    };

    class SPIRVUniformVariable : public SPIRVShaderVariable
    {
    public:

    };

    class SPIRVUniformBlock : public SPIRVShaderVariable
    {
    public:

    };

    class SPIRVPushConstant : public SPIRVShaderVariable
    {
    public:

    };

    class SPIRVStorageVariable : public SPIRVShaderVariable
    {
    public:

    };

    class SPIRVStorageBlock : public SPIRVShaderVariable
    {
    public:

    };
}