#pragma once

#include "CBufferBase.inc.h"
#include "AssertionFailed.h"

namespace GSZHU {
    template<typename INTERFACE, typename RENDER_DEVICE>
    CBufferBase<INTERFACE, RENDER_DEVICE>::CBufferBase(RENDER_DEVICE* Device, const DESC& Desc)
        : BASE(Device, Desc) {
        uint32_t AllowedBindFlags =
            BIND_VERTEX_BUFFER | BIND_INDEX_BUFFER | BIND_UNIFORM_BUFFER |
            BIND_SHADER_RESOURCE | BIND_STREAM_OUTPUT | BIND_UNORDERED_ACCESS | BIND_INDIRECT_DRAW_ARGS;
        CHECK((mDesc.BindFlags & ~AllowedBindFlags) == 0, "Incorrect bind flags specified (", BuffDesc.BindFlags & ~AllowedBindFlags, ").");

        if (mDesc.Mode == BUFFER_MODE_STRUCTURED || mDesc.Mode == BUFFER_MODE_FORMATTED) {
            CHECK(mDesc.ElementByteStride != 0, "Element stride cannot be zero for structured and formatted buffers");
        }

        uint64_t DeviceQueuesMask = mDevice->GetCommandQueueMask();
        CHECK((mDesc.CommandQueueMask & DeviceQueuesMask) != 0, "No bits in the command queue mask (0x", std::hex, mDesc.CommandQueueMask,
            ") correspond to one of ", Device->GetCommandQueueCount(), " available device command queues");
        mDesc.CommandQueueMask &= DeviceQueuesMask;
    }

    template<typename INTERFACE, typename RENDER_DEVICE>
    IBufferView* CBufferBase<INTERFACE, RENDER_DEVICE>::CreateView(const VIEW_DESC& ViewDesc) {
        CHECK(ViewDesc.ViewType != BUFFER_VIEW_UNDEFINED, "Buffer view type is not specified");
        if (ViewDesc.ViewType == BUFFER_VIEW_SHADER_RESOURCE)
            CHECK(mDesc.BindFlags & BIND_SHADER_RESOURCE, "Attempting to create SRV for buffer '", mDesc.Name, 
                  "' that was not created with BIND_SHADER_RESOURCE flag");
        else if (ViewDesc.ViewType == BUFFER_VIEW_UNORDERED_ACCESS)
            CHECK(mDesc.BindFlags & BIND_UNORDERED_ACCESS, "Attempting to create UAV for buffer '", mDesc.Name,
                  "' that was not created with BIND_UNORDERED_ACCESS flag");
        else
            UNEXPECTED("Unexpected buffer view type");

        return CreateViewInternal(ViewDesc, false);
    }

    template<typename INTERFACE, typename RENDER_DEVICE>
    IBufferView* CBufferBase<INTERFACE, RENDER_DEVICE>::GetDefaultView(EBUFFER_VIEW_TYPE ViewType) {
        if (ViewType == BUFFER_VIEW_SHADER_RESOURCE)
            return mDefaultSRV.get();
        else if (ViewType == BUFFER_VIEW_UNORDERED_ACCESS)
            return mDefaultUAV.get();
        else {
            UNEXPECTED("Unknown view type");
            return nullptr;
        }
    }

    template<typename INTERFACE, typename RENDER_DEVICE>
    bool CBufferBase<INTERFACE, RENDER_DEVICE>::CreateDefaultViews() {
        if (mDesc.BindFlags & BIND_UNORDERED_ACCESS && (mDesc.Mode == BUFFER_MODE_STRUCTURED || mDesc.Mode == BUFFER_MODE_RAW)) {
            VIEW_DESC ViewDesc;
            ViewDesc.ViewType = BUFFER_VIEW_UNORDERED_ACCESS;
            IBufferView* pUAV = CreateViewInternal(ViewDesc, true);
            mDefaultUAV.reset(static_cast<BUFFER_VIEW*>(pUAV));
            ASSERT(mDefaultUAV->GetDesc().ViewType == BUFFER_VIEW_UNORDERED_ACCESS);
        }

        if (mDesc.BindFlags & BIND_SHADER_RESOURCE && (mDesc.Mode == BUFFER_MODE_STRUCTURED || mDesc.Mode == BUFFER_MODE_RAW)) {
            VIEW_DESC ViewDesc;
            ViewDesc.ViewType = BUFFER_VIEW_SHADER_RESOURCE;
            IBufferView* pSRV =  CreateViewInternal(ViewDesc, true);
            mDefaultSRV.reset(static_cast<BUFFER_VIEW*>(pSRV));
            ASSERT(mDefaultSRV->GetDesc().ViewType == BUFFER_VIEW_SHADER_RESOURCE);
        }
    }

    template<typename INTERFACE, typename RENDER_DEVICE>
    void CBufferBase<INTERFACE, RENDER_DEVICE>::AdjustBufferViewDesc(VIEW_DESC& ViewDesc) const {
        if (ViewDesc.ByteWidth == 0) {
            CHECK(mDesc.SizeInBytes > ViewDesc.ByteOffset, "Byte offset exceeds buffer size");
            ViewDesc.ByteWidth = mDesc.SizeInBytes - ViewDesc.ByteOffset;
        }

        if (ViewDesc.ByteOffset + ViewDesc.ByteWidth > this->m_Desc.SizeInBytes)
            UNEXPECTED("Buffer view range is out of the buffer boundaries");

        if ((mDesc.BindFlags & BIND_UNORDERED_ACCESS) || (mDesc.BindFlags & BIND_SHADER_RESOURCE)) {
            if (mDesc.Mode == BUFFER_MODE_STRUCTURED || mDesc.Mode == BUFFER_MODE_FORMATTED) {
                ASSERT(mDesc.ElementByteStride != 0);
                ASSERT((ViewDesc.ByteOffset % this->m_Desc.ElementByteStride) == 0)
                ASSERT((ViewDesc.ByteWidth % this->m_Desc.ElementByteStride) == 0)
            }

            if (mDesc.Mode == BUFFER_MODE_FORMATTED && ViewDesc.Format.ValueType == VT_UNDEFINED)
                UNEXPECTED("Format must be specified when creating a view of a formatted buffer");

            if (mDesc.Mode == BUFFER_MODE_FORMATTED || (mDesc.Mode == BUFFER_MODE_RAW && ViewDesc.Format.ValueType != VT_UNDEFINED)) {
                if (ViewDesc.Format.NumComponents <= 0 || ViewDesc.Format.NumComponents > 4)
                    UNEXPECTED("Incorrect number of components");

                if (ViewDesc.Format.ValueType == VT_FLOAT32 || ViewDesc.Format.ValueType == VT_FLOAT16)
                    ViewDesc.Format.IsNormalized = false;

                auto ViewElementStride = sizeof(ViewDesc.Format.ValueType) * uint32_t(ViewDesc.Format.NumComponents);
                CHECK(!(mDesc.Mode == BUFFER_MODE_RAW && mDesc.ElementByteStride == 0),
                       "To enable formatted views of a raw buffer, element byte must be specified during buffer initialization")
                CHECK(ViewElementStride == mDesc.ElementByteStride, 
                      "Buffer element byte stride is not consistent")
            }

            if (mDesc.Mode == BUFFER_MODE_RAW && ViewDesc.Format.ValueType == VT_UNDEFINED) {
                CHECK((ViewDesc.ByteOffset % 16) == 0, "the offset of the first element from the start of the buffer must be a multiple of 16 bytes")
            }
        }
    }

    template<typename INTERFACE, typename RENDER_DEVICE>
    void CBufferBase<INTERFACE, RENDER_DEVICE>::SetState(ERESOURCE_STATE State) {
        mState = State;
    }

    template<typename INTERFACE, typename RENDER_DEVICE>
    ERESOURCE_STATE CBufferBase<INTERFACE, RENDER_DEVICE>::GetState() const {
        return mState;
    }

    template<typename INTERFACE, typename RENDER_DEVICE>
    bool CBufferBase<INTERFACE, RENDER_DEVICE>::CheckState(ERESOURCE_STATE State) const {
        CHECK((State & (State - 1)) == 0, "Single state is expected");
        ASSERT(State != RESOURCE_STATE_UNKNOWN, "Buffer state is unknown");
        return (mState & State) == State;
    }
}