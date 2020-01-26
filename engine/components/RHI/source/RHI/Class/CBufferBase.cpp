#include "RHI/Class/CBufferBase.h"
#include "RHI/IBufferView.h"
#include "Utils/Logger.h"

namespace Wanlix
{
    void CBufferBase::SetState(EResourceState state)
    {
        mResourceState = state;
    }

    EResourceState CBufferBase::GetState() const
    {
        return mResourceState; 
    }

    SharedPtr<IBufferView> CBufferBase::CreateView(const BufferViewDesc& desc)
    {
        CHECK_ERROR(desc.viewType != EBufferViewType::Undefined,
                   "Buffer view type is not specified");
        if (desc.viewType == EBufferViewType::ShaderResource)
        {
            CHECK_ERROR(HasBits(mDesc.bindFlags, EBindFlags::ShaderResource),
                       "Attempting to create SRV for buffer that was not created with EBindFlags::ShaderResource flag");
        }
        else if (desc.viewType == EBufferViewType::UnorderedAccess) 
        {
            CHECK_ERROR(HasBits(mDesc.bindFlags, EBindFlags::UnorderedAccess),
                       "");
        }
        else 
        {
            UNEXPECTED("Unexpected buffer view type");
            return nullptr;
        }

        return CreateViewInternal(desc, false);
    }

    SharedPtr<IBufferView> CBufferBase::GetDefaultView(EBufferViewType viewType)
    {
        switch (viewType)
        {
            case EBufferViewType::ShaderResource: return mDefaultSRV;
            case  EBufferViewType::UnorderedAccess: return mDefaultUAV;
            default: UNEXPECTED("Unknown view type"); return nullptr;
        }
    }

    Bool CBufferBase::IsInKnownState() const
    { 
        return mResourceState != EResourceState::Unknown; 
    }

    Bool CBufferBase::CheckState(EResourceState state) const
    {
        CHECK_ERROR(IsInKnownState(), "Buffer state is unknown");
        return mResourceState == state;
    }

    void CBufferBase::AdjustBufferViewDesc(BufferViewDesc& viewDesc) const
    {
        if (viewDesc.size == 0)
        {
            CHECK_ERROR(mDesc.sizeInBytes > viewDesc.offset,
                       "Byte offset (%d) exceeds buffer size (%d)", viewDesc.offset, mDesc.sizeInBytes);
        }

        if ((viewDesc.offset + viewDesc.offset) > mDesc.sizeInBytes)
        {
            LogFatal("Buffer view range [%d, %d] is out of the buffer boundaries [0, %d).",
                     viewDesc.offset, viewDesc.offset + viewDesc.size, mDesc.sizeInBytes);
        }

        if (HasBits(mDesc.bindFlags, EBindFlags::ShaderResource) ||
            HasBits(mDesc.bindFlags, EBindFlags::UnorderedAccess))
        {
            if (mDesc.bufferMode == EBufferMode::Structured ||
                mDesc.bufferMode == EBufferMode::Formatted)
            {
                CHECK_ERROR(mDesc.stride != 0, "Element byte stride is zero");
                if (viewDesc.offset % mDesc.stride != 0) 
                {
                    LogFatal("Buffer view byte offset (%d) is not multiple of element byte stride (%d).",
                             viewDesc.offset, mDesc.stride);
                }
                if (viewDesc.size % mDesc.stride != 0) 
                {
                    LogFatal("Buffer view byte size (%d) is not multiple of element byte stride (%d).",
                             viewDesc.size, mDesc.stride);
                }
            }

            if (mDesc.bufferMode == EBufferMode::Formatted &&
                viewDesc.format.dataType == EDataType::Undefined)
            {
                LogFatal("Format must be specified when creating a view of a formatted buffer");
            }

            if (mDesc.bufferMode == EBufferMode::Formatted ||
                (mDesc.bufferMode == EBufferMode::Raw && viewDesc.format.dataType != EDataType::Undefined))
            {
                if (viewDesc.format.components <= 0 || viewDesc.format.components > 4)
                {
                    LogFatal("Incorrect number of components (%d).", viewDesc.format.components);
                }

                if (viewDesc.format.dataType == EDataType::Float16 ||
                    viewDesc.format.dataType == EDataType::Float32 ||
                    viewDesc.format.dataType == EDataType::Float64) 
                {
                    viewDesc.format.isNormalized = false;
                }

                Uint stride = viewDesc.format.components * DataTypeSize(viewDesc.format.dataType);

                if (mDesc.bufferMode == EBufferMode::Raw && mDesc.stride == 0)
                {
                    LogFatal("To enable formatted views of a raw buffer, element byte must be specified during buffer initialization.");
                }
                if (stride != mDesc.stride)
                {
                    LogFatal("Buffer element byte stride (%d) is not consistent with the size (%d) defined by the format of the view.",
                             mDesc.stride, stride);
                }
            }

            if (mDesc.bufferMode == EBufferMode::Raw &&
                viewDesc.format.dataType == EDataType::Undefined)
            {
                if ((viewDesc.offset % 16) != 0) 
                {
                    LogFatal("When creating a RAW view, the offset of the first element from the start of the buffer (%d) must be a multiple of 16 bytes",
                             viewDesc.offset);
                }
            }
        }
    }
}