#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class SPIRVPipelineInput
    {
        friend class SPIRVShaderProgram;
    public:
        std::wstring const& GetName() const;
        uint32_t GetLocation() const;
        uint32_t GetSize() const;
        VkFormat GetFormat() const;

    private:
        SPIRVPipelineInput(
            const std::wstring& name,
            uint32_t location,
            VkFormat format);

    private:
        std::wstring mName;
        uint32_t mLocation = 0; // Input variable specified in vertex shader
        uint32_t mSize = 0;
        VkFormat mFormat = VK_FORMAT_UNDEFINED;
    };

    class SPIRVPipelineOutput
    {
        friend class SPIRVShaderProgram;
    public:
        std::wstring const& GetName() const;
        uint32_t GetLocation() const;
        uint32_t GetSize() const;
        VkFormat GetFormat() const;

    private:
        SPIRVPipelineOutput(
            const std::wstring& name,
            uint32_t location,
            VkFormat format);

    private:
        std::wstring mName;
        uint32_t mLocation = 0; // Input variable specified in vertex shader
        uint32_t mSize = 0;
        VkFormat mFormat = VK_FORMAT_UNDEFINED;
    };

    class SPIRVUniformVariable
    {
        friend class SPIRVShaderProgram;
    public:
        std::wstring const& GetName() const;
        uint32_t GetDescriptorSetID() const;
        uint32_t GetBindingID() const;
        uint32_t GetOffset() const;
        uint32_t GetSize() const;
        VkFormat GetFormat() const;
        VkShaderStageFlagBits GetStageFlags() const;

    private:
        std::wstring mName;
        uint32_t mDescriptorSetID = 0;
        uint32_t mBindingID = 0;
        uint32_t mOffset = 0;
        uint32_t mSize = 0;
        VkFormat mFormat = VK_FORMAT_UNDEFINED;
        VkShaderStageFlagBits mStageFlags = VK_SHADER_STAGE_ALL;
    };

    class SPIRVUniformBlock
    {

    };

    class SPIRVStorageVariable
    {

    };

    class SPIRVStorageBlock
    {

    };

    class SPIRVPushConstant
    {

    };

    class SPIRVAtomicCounter
    {

    };
}