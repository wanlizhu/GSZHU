#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <unordered_map>
#include "dejavu/core/preprocess.h"
#include "render_device_types.h"

namespace djv
{
    class IRenderDevice;

    DJV_API IRenderDevice* create_render_device();
    DJV_API void delete_render_device(IRenderDevice* device);

    class IRenderDevice
    {
    public:
        virtual ~IRenderDevice() = default;
        virtual void waitIdle() const = 0;
        virtual void setFramebuffer(GPUFramebuffer framebuffer) = 0;

        virtual GPUCommandBuffer createCommandBuffer() = 0;
        
        virtual GPUBuffer createBuffer(
            ResourceUsage  usage, 
            MemoryProperty memProp,
            size_t      size,
            const void* data
        ) = 0;

        virtual GPUImage  createImage(
            ResourceUsage  usage,
            MemoryProperty memProp,
            size_t width, 
            size_t height,
            Format format,
            uint32_t mipLevels,
            uint32_t arrayLayers,
            uint32_t sampleCount,
            const void* data
        ) = 0;

        virtual GPUImage  createImage(
            ResourceUsage      usage,
            MemoryProperty     memProp,
            const std::string& filePath
        ) = 0;

        virtual GPUBufferView createBufferView(
            GPUBuffer buffer,
            Format    format,
            size_t    offset,
            size_t    size
        ) = 0;

        virtual GPUImageView createImageView(
            GPUImage image,
            Format format
        ) = 0;

        virtual GPUSampler createSampler(
            Filter magFilter,
            Filter minFilter,
            MipmapMode  mipmapMode,
            AddressMode addressModeU,
            AddressMode addressModeV,
            AddressMode addressModeW,
            std::optional<float> maxAnisotropy,
            std::optional<CompareOp> compareOp
        ) = 0; 

        virtual GPUShaderProgram createShaderProgram(
            const std::vector<std::string>& shaderPaths,
            const std::vector<std::string>& entryPoints,
            const std::unordered_map<std::string, std::string>& macros
        ) = 0;

        virtual GPUPipeline createPipeline(
            GPUShaderProgram shaderProgram,
            GPURenderPass    renderPass,
            const VertexInputState&   vertexInput,
            const ViewportState*      viewport,
            const RasterizationState* rasterization,
            const DepthStencilState*  depthStencil,
            const ColorBlendState*    colorBlend,
            const std::optional<GPUPipeline>& parentPipeline,
            const std::optional<std::string>& pipelineCache
        ) = 0;

        virtual GPURenderPass createRenderPass(
            GPUFramebuffer framebuffer,
            const std::vector<AttachmentDescription>& colorAttachments,
            const std::optional<AttachmentDescription>& depthStencilAttachment
        ) = 0;

        virtual GPUFramebuffer createFramebuffer(
            const std::vector<GPUImageView>& colorTargets,
            const std::optional<GPUImageView>& depthStencilTarget
        ) = 0;

        virtual GPUSwapchain createSwapchain(
            void* window,
            uint32_t imageCount,
            Format desiredFormat
        ) = 0;
    };
}