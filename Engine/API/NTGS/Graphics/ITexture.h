#pragma once

#include <NTGS/Graphics/IDeviceObject.h>

namespace NTGS {
    class IDeviceContext;
    class ITextureView;

    struct TextureData {
        TextureSubResData* SubResources = nullptr;
        UINT NumSubResources = 0;
    };

    struct MappedTextureSubResource {
        void* Data = nullptr;
        UINT Stride = 0;
        UINT DepthStride = 0;
    };

    struct TextureFormatAttribs {
        const char* Name = "TEX_FORMAT_UNKNOWN"; // e.g. for TEX_FORMAT_RGBA8_UNORM format, this will be "TEX_FORMAT_RGBA8_UNORM"

        TEXTURE_FORMAT Format = TEX_FORMAT_UNKNOWN;
        UINT8 ComponentSize = 0;
        UINT8 NumComponents = 0;
        COMPONENT_TYPE ComponentType = COMPONENT_TYPE_UNDEFINED;

        bool IsTypeless = false;
        UINT8 BlockWidth = 0;
        UINT8 BlockHeight = 0;

        TextureFormatAttribs() = default;
        TextureFormatAttribs(const char* _Name,
            TEXTURE_FORMAT _Format,
            UINT8 _ComponentSize,
            UINT8 _NumComponents,
            COMPONENT_TYPE _ComponentType,
            bool _IsTypeless,
            UINT8 _BlockWidth,
            UINT8 _BlockHeight) noexcept
            : Name(_Name),
            Format(_Format),
            ComponentSize(_ComponentSize),
            NumComponents(_NumComponents),
            ComponentType(_ComponentType),
            IsTypeless(_IsTypeless),
            BlockWidth(_BlockWidth),
            BlockHeight(_BlockHeight) {}
    };

    struct DepthStencilClearValue {
        FLOAT Depth = 1.f;
        UINT8 Stencil = 0;

        DepthStencilClearValue() = default;
        DepthStencilClearValue(FLOAT _Depth, UINT8 _Stencil) noexcept 
            : Depth(_Depth), Stencil(_Stencil) {}
    };

    struct OptimizedClearValue {
        TEXTURE_FORMAT Format = TEX_FORMAT_UNKNOWN;
        FLOAT Color[4] = { 0, 0, 0, 0 };
        DepthStencilClearValue DepthStencil;

        bool operator == (const OptimizedClearValue& RHS) const {
            return Format == RHS.Format &&
                Color[0] == RHS.Color[0] &&
                Color[1] == RHS.Color[1] &&
                Color[2] == RHS.Color[2] &&
                Color[3] == RHS.Color[3] &&
                DepthStencil.Depth == RHS.DepthStencil.Depth &&
                DepthStencil.Stencil == RHS.DepthStencil.Stencil;
        }
    };

    struct TextureDesc : public IDeviceObjectAttribs {
        RESOURCE_DIMENSION Type = RESOURCE_DIM_UNDEFINED;
        UINT Width = 0;
        UINT Height = 0;
        union {
            UINT ArraySize = 1;
            UINT Depth;
        };
        TEXTURE_FORMAT Format = TEX_FORMAT_UNKNOWN;
        UINT MipLevels = 1;
        UINT SampleCount = 1;
        USAGE Usage = USAGE_DEFAULT;
        BIND_FLAGS BindFlags = BIND_NONE;
        CPU_ACCESS_FLAGS CPUAccessFlags = CPU_ACCESS_NONE;
        MISC_TEXTURE_FLAGS MiscFlags = MISC_TEXTURE_FLAG_NONE;
        OptimizedClearValue ClearValue;
        uint64_t CommandQueueMask = 1;

        TextureDesc() = default;
        TextureDesc(RESOURCE_DIMENSION  _Type,
                    UINT _Width,
                    UINT _Height,
                    UINT _ArraySizeOrDepth,
                    TEXTURE_FORMAT      _Format,
                    UINT _MipLevels = TextureDesc{}.MipLevels,
                    UINT _SampleCount = TextureDesc{}.SampleCount,
                    USAGE _Usage = TextureDesc{}.Usage,
                    BIND_FLAGS _BindFlags = TextureDesc{}.BindFlags,
                    CPU_ACCESS_FLAGS _CPUAccessFlags = TextureDesc{}.CPUAccessFlags,
                    MISC_TEXTURE_FLAGS _MiscFlags = TextureDesc{}.MiscFlags,
                    OptimizedClearValue _ClearValue = TextureDesc{}.ClearValue,
                    uint64_t _CommandQueueMask = TextureDesc{}.CommandQueueMask) 
            : Type(_Type),
              Width(_Width),
              Height(_Height),
              ArraySize(_ArraySizeOrDepth),
              Format(_Format),
              MipLevels(_MipLevels),
              SampleCount(_SampleCount),
              Usage(_Usage),
              BindFlags(_BindFlags),
              CPUAccessFlags(_CPUAccessFlags),
              MiscFlags(_MiscFlags),
              ClearValue(_ClearValue),
              CommandQueueMask(_CommandQueueMask) {}

        bool operator ==(const TextureDesc& RHS) const {
            return  // strcmp(Name, RHS.Name) == 0 &&
                Type == RHS.Type &&
                Width == RHS.Width &&
                Height == RHS.Height &&
                ArraySize == RHS.ArraySize &&
                Format == RHS.Format &&
                MipLevels == RHS.MipLevels &&
                SampleCount == RHS.SampleCount &&
                Usage == RHS.Usage &&
                BindFlags == RHS.BindFlags &&
                CPUAccessFlags == RHS.CPUAccessFlags &&
                MiscFlags == RHS.MiscFlags &&
                ClearValue == RHS.ClearValue &&
                CommandQueueMask == RHS.CommandQueueMask;
        }
    };

    struct TextureSubResData {
        const void* Data = nullptr; // If provided, SrcBuffer must be null
        class IBuffer* SrcBuffer = nullptr;
        UINT SrcOffset = 0;
        UINT Stride = 0;
        UINT DepthStride = 0;

        TextureSubResData() = default;
        TextureSubResData(void *_Data, UINT _Stride, UINT _DepthStride = 0) noexcept 
            : Data(_Data),
              SrcBuffer(nullptr),
              SrcOffset(0),
              Stride(_Stride),
              DepthStride(_DepthStride) {}
        TextureSubResData(IBuffer *_Buffer, UINT _SrcOffset, UINT _Stride, UINT _DepthStride = 0) noexcept 
            : Data(nullptr),
              SrcBuffer(_Buffer),
              SrcOffset(_SrcOffset),
              Stride(_Stride),
              DepthStride(_DepthStride)
        {}
    };

    struct TextureFormatInfo : public TextureFormatAttribs {
        bool Supported = false;
    };

    struct TextureFormatInfoExt : public TextureFormatInfo {
        bool Filterable = false;
        bool ColorRenderable = false;
        bool DepthRenderable = false;
        bool Tex1DFmt = false;
        bool Tex2DFmt = false;
        bool Tex3DFmt = false;
        bool TexCubeFmt = false;
        UINT SampleCounts = 0;
    };

    struct CopyTextureAttribs {
        ITexture* SrcTexture = nullptr;
        UINT SrcMipLevel = 0;
        UINT SrcSlice = 0;
        const Box* SrcBox = nullptr;
        RESOURCE_STATE_TRANSITION_MODE SrcTextureTransitionMode = RESOURCE_STATE_TRANSITION_MODE_NONE;
        ITexture* pDstTexture = nullptr;
        UINT DstMipLevel = 0;
        UINT DstSlice = 0;
        UINT DstX = 0;
        UINT DstY = 0;
        UINT DstZ = 0;
        RESOURCE_STATE_TRANSITION_MODE DstTextureTransitionMode = RESOURCE_STATE_TRANSITION_MODE_NONE;

        CopyTextureAttribs() noexcept {}
        CopyTextureAttribs(ITexture* _pSrcTexture,
            RESOURCE_STATE_TRANSITION_MODE _SrcTextureTransitionMode,
            ITexture* _pDstTexture,
            RESOURCE_STATE_TRANSITION_MODE _DstTextureTransitionMode) noexcept :
            SrcTexture(_pSrcTexture),
            SrcTextureTransitionMode(_SrcTextureTransitionMode),
            pDstTexture(_pDstTexture),
            DstTextureTransitionMode(_DstTextureTransitionMode)
        {}
    };

    class ITexture : public IDeviceObject {
    public:
        virtual const TextureDesc& GetDesc()const = 0;
        virtual ITextureView* CreateView(const struct TextureViewDesc& ViewDesc) = 0;
        virtual ITextureView* GetDefaultView(TEXTURE_VIEW_TYPE ViewType) = 0;
        virtual void* GetNativeHandle() = 0;
        virtual void SetState(RESOURCE_STATE State) = 0;
        virtual RESOURCE_STATE GetState() const = 0;
    };
}