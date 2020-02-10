#pragma once

#include "BasicTypes.h"
#include "RHIUtilities.h"
#include "RHITexture.h"

namespace Wanli
{
    struct RHIRenderPassCreateInfo
    {
        struct TargetEntry
        {
            RHITexture* target = nullptr;
            RHITexture* resolveTarget = nullptr;

            VkAttachmentLoadOp loadAction = VK_ATTACHMENT_LOAD_OP_CLEAR;
            VkAttachmentStoreOp storeAction = VK_ATTACHMENT_STORE_OP_STORE;
        };

        TargetEntry colorTargets[MaxSimultaneousRenderTargets];
        TargetEntry depthStencilTarget;
        int colorTargetCount = 0;
        bool multiview = false;
    };

    class DLLDECL RHIRenderPass : public RHIDeviceObject
    {
    public:
        RHIRenderPass(RHIDevice* device, 
                      RHIRenderPassCreateInfo& layout);
        virtual ~RHIRenderPass();

        virtual void Destroy() override;
        
        inline VkRenderPass GetHandle() const { return mRenderPass; }
        
    protected:
        VkRenderPass mRenderPass = VK_NULL_HANDLE;
    };
}