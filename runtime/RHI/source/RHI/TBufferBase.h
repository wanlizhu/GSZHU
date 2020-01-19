#pragma once

#include "RHI/IBuffer.h"
#include "RHI/TDeviceObjectBase.h"

namespace Wanlix
{
    template<typename _Interface_>
    class TBufferBase : public TDeviceObjectBase<_Interface_>
    {
        static_assert(std::is_base_of_v<IBuffer, _Interface_> || std::is_same_v<IBuffer, _Interface_>);
        using BaseClass = TDeviceObjectBase<_Interface_>;
        using DeviceType = typename _Interface_::Device;
        using DescType = typename _Interface_::Desc;
        using ViewType = typename _Interface_::View;
        using ViewDescType = typename ViewType::Desc;

    public:
        virtual SharedPtr<IBufferView> CreateView(const ViewDescType& desc) override;
        virtual SharedPtr<IBufferView> GetDefaultView(EBufferViewType type) override;
        virtual void SetState(EResourceState state) override final { mResourceState = state; }
        virtual EResourceState GetState() override final { return mResourceState; }
        
        inline Bool IsInKnownState() const { return mResourceState != EResourceState::UnknownState; }
        inline Bool CheckState(EResourceState state) const;

    protected:
        TBufferBase(DeviceType* device,
                    const DescType& desc,
                    const String& name)
            : BaseClass(device, desc, name)
        {}

        virtual SharedPtr<IBufferView> CreateViewInternal(const ViewDescType& desc, Bool isDefault) = 0;
        void AdjustBufferViewDesc(ViewDescType& desc) const;

    protected:
        EResourceState mResourceState = EResourceState::Unknown;
        SharedPtr<ViewType> mDefaultUAV;
        SharedPtr<ViewType> mDefaultSRV;
    };



    template<typename _Device_, typename _Interface_>
    SharedPtr<IBufferView> TBufferBase<_Device_, _Interface_>::CreateView(const ViewDescType& desc)
    {
        CheckError(desc.viewType != EBufferViewType::Undefined,
                   "Buffer view type is not specified");
        if (desc.viewType == EBufferViewType::ShaderResource) {
            CheckError(HasBits(mDesc.bindFlags, EBindFlags::ShaderResource),
                       "Attempting to create SRV for buffer that was not created with EBindFlags::ShaderResource flag");
        }
        else if (desc.viewType == EBufferViewType::UnorderedAccess) {
            CheckError(HasBits(mDesc.bindFlags, EBindFlags::UnorderedAccess),
                       "");
        }
        else {
            Unexpected("Unexpected buffer view type");
            return nullptr;
        }

        return CreateViewInternal(desc, false);
    }

    template<typename _Device_, typename _Interface_>
    SharedPtr<IBufferView> TBufferBase<_Device_, _Interface_>::GetDefaultView(EBufferViewType type)
    {
        switch (type)
        {
        case EBufferViewType::ShaderResource: return mDefaultSRV;
        case  EBufferViewType::UnorderedAccess: return mDefaultUAV;
        default: Unexpected("Unknown view type"); return nullptr;
        }
    }

    template<typename _Device_, typename _Interface_>
    inline Bool TBufferBase<_Device_, _Interface_>::CheckState(EResourceState state) const
    {
        CheckError((state & (state - 1)) == 0, "Single state is expected");
        CheckError(IsInKnownState(), "Buffer state is unknown");
        return (mResourceState & state) == state;
    }

    template<typename _Device_, typename _Interface_>
    void TBufferBase<_Device_, _Interface_>::AdjustBufferViewDesc(ViewDescType& viewDesc) const
    {
        if (viewDesc.size == 0)
        {
            CheckError(mDesc.sizeInBytes > desc.offset, 
                       "Byte offset (%d) exceeds buffer size (%d)", desc.offset, mDesc.sizeInBytes);
        }

        if (viewDesc.offse + viewDesc.offset > mDesc.sizeInBytes)
        {
            LogFatal("Buffer view range [%d, %d] is out of the buffer boundaries [0, %d).",
                     viewDesc.offset, viewDesc.offset + viewDesc.size, mDesc.sizeInBytes);
        }

        if (HasBits(mDesc.bindFlags, EBindFlags::ShaderResource) ||
            HasBits(mDesc, bindFlags, EBindFlags::UnorderedAccess))
        {
            if (mDesc.mode == EBufferMode::Structured ||
                mDesc.mode == EBufferMode::Formatted)
            {
                CheckError(mDesc.stride != 0, "Element byte stride is zero");
                if (viewDesc.offset % mDesc.stride != 0) {
                    LogFatal("Buffer view byte offset (%d) is not multiple of element byte stride (%d).", 
                             viewDesc.offset, mDesc.stride);
                }
                if (viewDesc.size % mDesc.stride != 0) {
                    LogFatal("Buffer view byte size (%d) is not multiple of element byte stride (%d).",
                             viewDesc.size, mDesc.stride);
                }
            }

            if (mDesc.mode == EBufferMode::Formatted &&
                viewDesc.format.dataType == EDataType::Undefined) 
            {
                LogFatal("Format must be specified when creating a view of a formatted buffer");
            }

            if (mDesc.mode == EBufferMode::Formatted ||
                (mDesc.mode == EBufferMode::Raw && viewDesc.format.dataType != EDataType::Undefined))
            {
                if (viewDesc.format.components <= 0 || viewDesc.format.components > 4) {
                    LogFatal("Incorrect number of components (%d).", viewDesc.format.components);
                }

                if (viewDesc.format.dataType == EDataType::Float32 ||
                    viewDesc.format.dataType == EDataType::Float32 ||
                    viewDesc.format.dataType == EDataType::Float64) {
                    viewDesc.format.isNormalized = false;
                }

                Uint stride = viewDesc.format.components * DataTypeSize(viewDesc.format.dataType);

                if (mDesc.mode == EBufferMode::Raw && mDesc.stride == 0) {
                    LogFatal("To enable formatted views of a raw buffer, element byte must be specified during buffer initialization.");
                }
                if (stride != mDesc.stride) {
                    LogFatal("Buffer element byte stride (%d) is not consistent with the size (%d) defined by the format of the view.",
                             mDesc.stride, stride);
                }
            }

            if (mDesc.mode == EBufferMode::Raw && 
                viewDesc.format.dataType == EDataType::Undefined)
            {
                if ((viewDesc.offset % 16) != 0) {
                    LogFatal("When creating a RAW view, the offset of the first element from the start of the buffer (%d) must be a multiple of 16 bytes",
                             viewDesc.offset);
                }
            }
        }
    }
}