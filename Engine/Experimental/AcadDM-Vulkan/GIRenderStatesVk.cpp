#include "GIRenderStatesVk.h"
#include "SPIRVShaderProgram.h"

namespace AutoCAD::Graphics::Engine
{
    GIVertexInputStateVk::GIVertexInputStateVk()
    {}

    GIVertexInputStateVk::GIVertexInputStateVk(SharedPtr<SPIRVShaderProgram> program)
    {
        // TODO
    }

    ERenderState GIVertexInputStateVk::GetRenderStateType() const
    {
        return ERenderState::VertexInput;
    }

    GIVertexInputStateVk& GIVertexInputStateVk::AddInputAttribute(const VkVertexInputAttributeDescription& attrib)
    {
        mInputAttributes.emplace_back(attrib);
        return *this;
    }

    GIVertexInputStateVk& GIVertexInputStateVk::AddInputBinding(const VkVertexInputBindingDescription& binding)
    {
        mInputBindings.emplace_back(binding);
        return *this;
    }

    ERenderState GIInputAssemblyStateVk::GetRenderStateType() const
    {
        return ERenderState::InputAssembly;
    }

    GIInputAssemblyStateVk& GIInputAssemblyStateVk::SetPrimitiveTopology(VkPrimitiveTopology topology)
    {
        mTopology = topology;
        return *this;
    }

    GIInputAssemblyStateVk& GIInputAssemblyStateVk::EnablePrimitiveRestart(bool enable)
    {
        mPrimitiveRestartEnable = enable ? VK_TRUE : VK_FALSE;
        return *this;
    }

    ERenderState GITessellationStateVk::GetRenderStateType() const
    {
        return ERenderState::Tessellation;
    }

    GITessellationStateVk& GITessellationStateVk::SetPatchControlPoint(uint32_t points)
    {
        mPatchControlPoints = points;
        return *this;
    }

    ERenderState GIViewportStateVk::GetRenderStateType() const
    {
        return ERenderState::Viewport;
    }

    GIViewportStateVk& GIViewportStateVk::AddViewport(const VkViewport& viewport)
    {
        mViewports.push_back(viewport);
        return *this;
    }

    GIViewportStateVk& GIViewportStateVk::AddScissor(const VkRect2D& scissor)
    {
        mScissors.push_back(scissor);
        return *this;
    }

    ERenderState GIRasterizationStateVk::GetRenderStateType() const
    {
        return ERenderState::Rasterization;
    }

    GIRasterizationStateVk& GIRasterizationStateVk::EnableDepthClamp(bool enable)
    {
        mDepthClampEnable = enable ? VK_TRUE : VK_FALSE;
        return *this;
    }

    GIRasterizationStateVk& GIRasterizationStateVk::EnableRasterizerDiscard(bool enable)
    {
        mRasterizerDiscardEnable = enable ? VK_TRUE : VK_FALSE;
        return *this;
    }

    GIRasterizationStateVk& GIRasterizationStateVk::SetPolygonMode(VkPolygonMode polygonMode)
    {
        mPolygonMode = polygonMode;
        return *this;
    }

    GIRasterizationStateVk& GIRasterizationStateVk::SetCullMode(VkCullModeFlags cullMode)
    {
        mCullMode = cullMode;
        return *this;
    }

    GIRasterizationStateVk& GIRasterizationStateVk::SetFrontFace(VkFrontFace frontFace)
    {
        mFrontFace = frontFace;
        return *this;
    }

    GIRasterizationStateVk& GIRasterizationStateVk::EnableDepthBias(bool enable)
    {
        mDepthBiasEnable = enable ? VK_TRUE : VK_FALSE;
        return *this;
    }

    GIRasterizationStateVk& GIRasterizationStateVk::SetDepthBiasConstantFactor(float constant)
    {
        mDepthBiasConstantFactor = constant;
        return *this;
    }

    GIRasterizationStateVk& GIRasterizationStateVk::SetDepthBiasClamp(float clamp)
    {
        mDepthBiasClamp = clamp;
        return *this;
    }

    GIRasterizationStateVk& GIRasterizationStateVk::SetDepthBiasSlopeFactor(float slope)
    {
        mDepthBiasSlopeFactor = slope;
        return *this;
    }

    GIRasterizationStateVk& GIRasterizationStateVk::SetLineWidth(float width)
    {
        mLineWidth = width;
        return *this;
    }

    ERenderState GIMultisampleStateVk::GetRenderStateType() const
    {
        return ERenderState::Multisample;
    }

    GIMultisampleStateVk& GIMultisampleStateVk::SetSampleCount(VkSampleCountFlagBits count)
    {
        mSampleCount = count;
        return *this;
    }

    GIMultisampleStateVk& GIMultisampleStateVk::EnableSampleShading(bool enable)
    {
        mSampleShadingEnable = enable ? VK_TRUE : VK_FALSE;
        return *this;
    }

    GIMultisampleStateVk& GIMultisampleStateVk::SetMinSampleShading(float minSampleShading)
    {
        mMinSampleShading = minSampleShading;
        return *this;
    }

    GIMultisampleStateVk& GIMultisampleStateVk::SetSampleMask(const VkSampleMask* sampleMask)
    {
        mSampleMask = sampleMask;
        return *this;
    }

    GIMultisampleStateVk& GIMultisampleStateVk::EnableAlphaToCoverage(bool enable)
    {
        mAlphaToCoverageEnable = enable ? VK_TRUE : VK_FALSE;
        return *this;
    }

    GIMultisampleStateVk& GIMultisampleStateVk::EnableAlphaToOne(bool enable)
    {
        mAlphaToOneEnable = enable ? VK_TRUE : VK_FALSE;
        return *this;
    }

    ERenderState GIDepthStencilStateVk::GetRenderStateType() const
    {
        return ERenderState::DepthStencil;
    }

    GIDepthStencilStateVk& GIDepthStencilStateVk::EnableDepthTest(bool enable)
    {
        mDepthTestEnable = enable ? VK_TRUE : VK_FALSE;
        return *this;
    }

    GIDepthStencilStateVk& GIDepthStencilStateVk::EnableDepthWrite(bool enable)
    {
        mDepthWriteEnable = enable ? VK_TRUE : VK_FALSE;
        return *this;
    }

    GIDepthStencilStateVk& GIDepthStencilStateVk::SetDepthCompareOp(VkCompareOp compareOp)
    {
        mDepthCompareOp = compareOp;
        return *this;
    }

    GIDepthStencilStateVk& GIDepthStencilStateVk::EnableDepthBoundsTest(bool enable)
    {
        mDepthBoundsTestEnable = enable ? VK_TRUE : VK_FALSE;
        return *this;
    }

    GIDepthStencilStateVk& GIDepthStencilStateVk::EnableStencilTest(bool enable)
    {
        mStencilTestEnable = enable ? VK_TRUE : VK_FALSE;
        return *this;
    }

    GIDepthStencilStateVk& GIDepthStencilStateVk::SetFrontStencilState(VkStencilOpState frontState)
    {
        mFront = frontState;
        return *this;
    }

    GIDepthStencilStateVk& GIDepthStencilStateVk::SetBackStencilState(VkStencilOpState backState)
    {
        mBack = backState;
        return *this;
    }

    GIDepthStencilStateVk& GIDepthStencilStateVk::SetMinDepthBounds(float minDepthBounds)
    {
        mMinDepthBounds = minDepthBounds;
        return *this;
    }

    GIDepthStencilStateVk& GIDepthStencilStateVk::SetMaxDepthBounds(float maxDepthBounds)
    {
        mMaxDepthBounds = maxDepthBounds;
        return *this;
    }

    ERenderState GIColorBlendStateVk::GetRenderStateType() const
    {
        return ERenderState::ColorBlend;
    }

    GIColorBlendStateVk& GIColorBlendStateVk::EnableLogicOp(bool enable)
    {
        mLogicOpEnable = enable ? VK_TRUE : VK_FALSE;
        return *this;
    }

    GIColorBlendStateVk& GIColorBlendStateVk::SetLogicalOp(VkLogicOp logicOp)
    {
        mLogicOp = logicOp;
        return *this;
    }

    GIColorBlendStateVk& GIColorBlendStateVk::AddAttachmentState(const VkPipelineColorBlendAttachmentState& state)
    {
        mAttachments.push_back(state);
        return *this;
    }

    GIColorBlendStateVk& GIColorBlendStateVk::SetBlendConstants(const float* constants)
    {
        std::memcpy(mBlendConstants, constants, 4 * sizeof(float));
        return *this;
    }

}