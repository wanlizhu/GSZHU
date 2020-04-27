#pragma once

#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    enum class EResourceType
    {
        Buffer,
        Image,
    };

    enum class EResourceState
    {
        Undefined=0,
        PreInitialized,
        General,
        VertexBuffer,
        IndexBuffer,
        UniformBuffer,
        StorageBuffer,
        InputAttachment,
        ColorAttachment,
        DepthStencilAttachment_RW,
        DepthStencilAttachment_RO,
        UniformImage,
        StorageImage,
        TransferSrc,
        TransferDst,
        ResolveSrc,
        ResolveDst,
        Present,
        StreamOut,
        IndirectArg,
        Predication,
        AccelerationStructure,
    };

    class GIResourceVk : public GIDeviceObjectVk
    {
    public:
        struct StateInfo
        {
            EResourceState state;
            VkShaderStageFlagBits stage;
            VkAccessFlagBits access;
            std::optional<VkImageLayout> imageLayout;
        };

        static StateInfo GetStateInfo(EResourceState state);

        virtual ~GIResourceVk();
        virtual EResourceType GetResourceType() const = 0;
        virtual EResourceState GetResourceState() const = 0;
        virtual void TransitionState(const EResourceState& newState) = 0;
        virtual void SetOnDestroyCallback(const std::function<void()>& func) = 0;
        
    protected:
        GIResourceVk(SharedPtr<GIDeviceVk> device);
    };
}