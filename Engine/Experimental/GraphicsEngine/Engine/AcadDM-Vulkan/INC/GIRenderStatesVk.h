#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class SPIRVShaderProgram;

    enum class ERenderState
    {
        Undefined = 0,
        VertexInput,
        InputAssembly,
        Tessellation,
        Viewport,
        Rasterization,
        Multisample,
        DepthStencil,
        ColorBlend,
    };

    class GIIRenderStateVk
    {
    public:
        virtual ERenderState GetRenderStateType() const = 0;
    };

    class GIVertexInputStateVk : public GIIRenderStateVk
    {
        friend class GIGraphicsPipelineBuilderVk;
    public:
        // Add vertex input manually later
        GIVertexInputStateVk(); 
        // Add vertex input from shader reflection
        GIVertexInputStateVk(SharedPtr<SPIRVShaderProgram> program);
        virtual ERenderState GetRenderStateType() const override final;

        GIVertexInputStateVk& AddInputAttribute(const VkVertexInputAttributeDescription& attrib);
        GIVertexInputStateVk& AddInputBinding(const VkVertexInputBindingDescription& binding);

    private:
        std::vector<VkVertexInputAttributeDescription> mInputAttributes;
        std::vector<VkVertexInputBindingDescription> mInputBindings;
    };

    class GIInputAssemblyStateVk : public GIIRenderStateVk
    {
        friend class GIGraphicsPipelineBuilderVk;
    public:
        virtual ERenderState GetRenderStateType() const override final;

        GIInputAssemblyStateVk& SetPrimitiveTopology(VkPrimitiveTopology topology);
        GIInputAssemblyStateVk& EnablePrimitiveRestart(bool enable = true);

    private:
        VkPrimitiveTopology mTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        VkBool32 mPrimitiveRestartEnable = VK_FALSE;
    };

    class GITessellationStateVk : public GIIRenderStateVk
    {
        friend class GIGraphicsPipelineBuilderVk;
    public:
        virtual ERenderState GetRenderStateType() const override final;

        GITessellationStateVk& SetPatchControlPoint(uint32_t points);

    private:
        uint32_t mPatchControlPoints = 0;
    };

    class GIViewportStateVk : public GIIRenderStateVk
    {
        friend class GIGraphicsPipelineBuilderVk;
    public:
        virtual ERenderState GetRenderStateType() const override final;

        GIViewportStateVk& AddViewport(const VkViewport& viewport);
        GIViewportStateVk& AddScissor(const VkRect2D& scissor);

    private:
        std::vector<VkViewport> mViewports;
        std::vector<VkRect2D> mScissors;
    }; 

    class GIRasterizationStateVk : public GIIRenderStateVk
    {
        friend class GIGraphicsPipelineBuilderVk;
    public:
        virtual ERenderState GetRenderStateType() const override final;

        GIRasterizationStateVk& EnableDepthClamp(bool enable = true);
        GIRasterizationStateVk& EnableRasterizerDiscard(bool enable = true);
        GIRasterizationStateVk& SetPolygonMode(VkPolygonMode polygonMode);
        GIRasterizationStateVk& SetCullMode(VkCullModeFlags cullMode);
        GIRasterizationStateVk& SetFrontFace(VkFrontFace frontFace);
        GIRasterizationStateVk& EnableDepthBias(bool enable = true);
        GIRasterizationStateVk& SetDepthBiasConstantFactor(float constant);
        GIRasterizationStateVk& SetDepthBiasClamp(float clamp);
        GIRasterizationStateVk& SetDepthBiasSlopeFactor(float slope);
        GIRasterizationStateVk& SetLineWidth(float width);

    private:
        VkBool32 mDepthClampEnable = VK_FALSE;
        VkBool32 mRasterizerDiscardEnable = VK_FALSE;
        VkPolygonMode mPolygonMode = VK_POLYGON_MODE_FILL;
        VkCullModeFlags mCullMode = VK_CULL_MODE_BACK_BIT;
        VkFrontFace mFrontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        VkBool32 mDepthBiasEnable = VK_FALSE;
        float mDepthBiasConstantFactor;
        float mDepthBiasClamp;
        float mDepthBiasSlopeFactor;
        float mLineWidth = 1.0f;
    };

    class GIMultisampleStateVk : public GIIRenderStateVk
    {
        friend class GIGraphicsPipelineBuilderVk;
    public:
        virtual ERenderState GetRenderStateType() const override final;

        GIMultisampleStateVk& SetSampleCount(VkSampleCountFlagBits count);
        GIMultisampleStateVk& EnableSampleShading(bool enable = true);
        GIMultisampleStateVk& SetMinSampleShading(float minSampleShading);
        GIMultisampleStateVk& SetSampleMask(const VkSampleMask* sampleMask);
        GIMultisampleStateVk& EnableAlphaToCoverage(bool enable = true);
        GIMultisampleStateVk& EnableAlphaToOne(bool enable = true);

    private:
        VkSampleCountFlagBits mSampleCount = VK_SAMPLE_COUNT_1_BIT;
        VkBool32 mSampleShadingEnable = VK_FALSE;
        float mMinSampleShading;
        const VkSampleMask* mSampleMask = nullptr;
        VkBool32 mAlphaToCoverageEnable = VK_FALSE;
        VkBool32 mAlphaToOneEnable = VK_FALSE;
    };

    class GIDepthStencilStateVk : public GIIRenderStateVk
    {
        friend class GIGraphicsPipelineBuilderVk;
    public:
        virtual ERenderState GetRenderStateType() const override final;

        GIDepthStencilStateVk& EnableDepthTest(bool enable = true);
        GIDepthStencilStateVk& EnableDepthWrite(bool enable = true);
        GIDepthStencilStateVk& SetDepthCompareOp(VkCompareOp compareOp);
        GIDepthStencilStateVk& EnableDepthBoundsTest(bool enable = true);
        GIDepthStencilStateVk& EnableStencilTest(bool enable = true);
        GIDepthStencilStateVk& SetFrontStencilState(VkStencilOpState frontState);
        GIDepthStencilStateVk& SetBackStencilState(VkStencilOpState backState);
        GIDepthStencilStateVk& SetMinDepthBounds(float minDepthBounds);
        GIDepthStencilStateVk& SetMaxDepthBounds(float maxDepthBounds);

    private:
        VkBool32 mDepthTestEnable = VK_TRUE;
        VkBool32 mDepthWriteEnable = VK_TRUE;
        VkCompareOp mDepthCompareOp = VK_COMPARE_OP_LESS;
        VkBool32 mDepthBoundsTestEnable = VK_FALSE;
        VkBool32 mStencilTestEnable = VK_TRUE;
        VkStencilOpState mFront;
        VkStencilOpState mBack;
        float mMinDepthBounds;
        float mMaxDepthBounds;
    };

    class GIColorBlendStateVk : public GIIRenderStateVk
    {
        friend class GIGraphicsPipelineBuilderVk;
    public:
        virtual ERenderState GetRenderStateType() const override final;

        GIColorBlendStateVk& EnableLogicOp(bool enable = true);
        GIColorBlendStateVk& SetLogicalOp(VkLogicOp logicOp);
        GIColorBlendStateVk& AddAttachmentState(const VkPipelineColorBlendAttachmentState& state);
        GIColorBlendStateVk& SetBlendConstants(const float* constants);

    private:
        VkBool32 mLogicOpEnable = VK_FALSE;
        VkLogicOp mLogicOp;
        std::vector<VkPipelineColorBlendAttachmentState> mAttachments;
        float mBlendConstants[4];
    };
}