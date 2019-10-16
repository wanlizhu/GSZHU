#pragma once

#include "IDeviceResource.h"

namespace GE2::RHI
{
    class GE2_IMPEXP IDeviceState : public IDeviceObject
    {
    public:
        using SharedPtr = std::shared_ptr<IDeviceState>;
        using WeakPtr   = std::weak_ptr<IDeviceState>;

        IDeviceState() = default;
        
        // Abstract virtual methods inherited from IDeviceObject
        virtual ~IDeviceState() = 0;
        virtual bool   Initialize() = 0;
        virtual void   Destroy() = 0;
        virtual CSTR   GetName() const = 0;
        virtual HANDLE GetHandle() const = 0;
    };

    enum class ECullMode : uint32_t
    {
        None,
        Front,
        Back,
    };

    enum class EFillMode : uint32_t
    {
        Wireframe,
        Solid,
    };

    class GE2_IMPEXP RasterizerState : public IDeviceState
    {
    public:
        using SharedPtr = std::shared_ptr<RasterizerState>;
        using WeakPtr   = std::weak_ptr<RasterizerState>;

        RasterizerState() = default;

        virtual ~RasterizerState() override;
        virtual bool   Initialize() override;
        virtual void   Destroy() override;
        virtual CSTR   GetName() const override;
        virtual HANDLE GetHandle() const override;

        ECullMode GetCullMode() const;
        EFillMode GetFillMode() const;
        bool      IsFrontCCW() const;
        float     GetSlopedScaledDepthBias() const;
        int32_t   GetDepthBias() const;
        bool      IsClampDepthEnabled() const;
        bool      IsScissorEnabled() const;
        bool      IsLinesAAEnabled() const;
        uint32_t  GetForcedSampleCount() const;
        
    private:
        ECullMode mCullMode = ECullMode::Back;
        EFillMode mFillMode = EFillMode::Solid;
        bool      mIsFrontCCW = true;
        float     mSlopeScaledDepthBias = 0.f;
        int32_t   mDepthBias = 0;
        bool      mEnableClampDepth = false;
        bool      mEnableScissor = false;
        bool      mEnableLinesAA = true;
        uint32_t  mForcedSampleCount = 0;
    };

    enum class EBlendOp : uint32_t
    {
        Add,
        Subtract,
        ReverseSubtract,
        Min,
        Max,
    };

    enum class EBlendFunc : uint32_t
    {
        Zero,                   ///< (0, 0, 0, 0)
        One,                    ///< (1, 1, 1, 1)
        SrcColor,               ///< The fragment-shader output color
        OneMinusSrcColor,       ///< One minus the fragment-shader output color
        DstColor,               ///< The render-target color
        OneMinusDstColor,       ///< One minus the render-target color
        SrcAlpha,               ///< The fragment-shader output alpha value
        OneMinusSrcAlpha,       ///< One minus the fragment-shader output alpha value
        DstAlpha,               ///< The render-target alpha value
        OneMinusDstAlpha,       ///< One minus the render-target alpha value
        BlendFactor,            ///< Constant color, set using Desc#SetBlendFactor()
        OneMinusBlendFactor,    ///< One minus constant color, set using Desc#SetBlendFactor()
        SrcAlphaSaturate,       ///< (f, f, f, 1), where f = min(fragment shader output alpha, 1 - render-target pixel alpha)
        Src1Color,              ///< Fragment-shader output color 1
        OneMinusSrc1Color,      ///< One minus fragment-shader output color 1
        Src1Alpha,              ///< Fragment-shader output alpha 1
        OneMinusSrc1Alpha       ///< One minus fragment-shader output alpha 1
    };

    class GE2_IMPEXP BlendState : public IDeviceState
    {
    public:
        using SharedPtr = std::shared_ptr<BlendState>;
        using WeakPtr   = std::weak_ptr<BlendState>;
        struct BlendEquation 
        {
            EBlendOp   op = EBlendOp::Add;
            EBlendFunc source = EBlendFunc::One;
            EBlendFunc dest = EBlendFunc::Zero;
        };

        BlendState() = default;

        virtual ~BlendState() override;
        virtual bool   Initialize() override;
        virtual void   Destroy() override;
        virtual CSTR   GetName() const override;
        virtual HANDLE GetHandle() const override;

        bool          IsBlendEnabled() const;
        bool          IsIndependentBlendEnabled() const;
        bool          IsAlphaToCoverageEnabled() const;
        FLOAT4        GetConstantBlendFactor() const;
        BlendEquation GetBlendEquationRGB() const;
        BlendEquation GetBlendEquationALpha() const;

    private:
        bool   mEnableBlend = false;
        bool   mEnableIndependentBlend = false;
        bool   mEnableAlphaToCoverage = false;
        FLOAT4 mConstantBlendFactor = { 1.0 };
        BlendEquation mRGB;
        BlendEquation mAlpha;
    };

    enum class EStencilFace : uint32_t
    {
        All,
        Front,
        Back,
    };

    enum class EComparisonFunc : uint32_t
    {
        Disabled = 0,
        Never,
        Always,
        Less,
        Equal,
        NotEqual,
        Greater,
        GreaterEqual,
    };

    enum class EStencilOp : uint32_t
    {
        Keep,               ///< Keep the stencil value
        Zero,               ///< Set the stencil value to zero
        Replace,            ///< Replace the stencil value with the reference value
        Increase,           ///< Increase the stencil value by one, wrap if necessary
        IncreaseSaturate,   ///< Increase the stencil value by one, clamp if necessary
        Decrease,           ///< Decrease the stencil value by one, wrap if necessary
        DecreaseSaturate,   ///< Decrease the stencil value by one, clamp if necessary
        Invert              ///< Invert the stencil data (bitwise not)
    };

    struct StencilDesc
    {
        EComparisonFunc func          = EComparisonFunc::Always;
        EStencilOp stencilFailOp      = EStencilOp::Keep;
        EStencilOp depthFailOp        = EStencilOp::Keep;
        EStencilOp depthStencilPassOp = EStencilOp::Keep;
    };

    class GE2_IMPEXP DepthStencilState : public IDeviceObject
    {
    public:
        using SharedPtr = std::shared_ptr<DepthStencilState>;
        using WeakPtr   = std::weak_ptr<DepthStencilState>;

        DepthStencilState() = default;

        virtual ~DepthStencilState() override;
        virtual bool   Initialize() override;
        virtual void   Destroy() override;
        virtual CSTR   GetName() const override;
        virtual HANDLE GetHandle() const override;

    private:
        bool mEnableDepth = true;
        bool mEnableDepthWrite = true;
        EComparisonFunc mDepthFunc = EComparisonFunc::Less;

        bool        mEnableStencil = false;
        StencilDesc mStencilFront;
        StencilDesc mStencilBack;
        uint8_t     mStencilReadMask  = (uint8_t)-1;
        uint8_t     mStencilWriteMask = (uint8_t)-1;
        uint8_t     mStencilRef = 0;
    };
}