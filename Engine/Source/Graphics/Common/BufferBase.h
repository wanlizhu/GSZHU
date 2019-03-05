#pragma once

#include <NTGS/Graphics/IBuffer.h>
#include <NTGS/Graphics/GraphicsTypes.h>
#include <NTGS/Core/Utilities.h>
#include "DeviceObjectBase.h"

namespace NTGS {
    template<typename BaseInterface, typename RenderDeviceType, typename BufferViewType>
    class BufferBase : public DeviceObjectBase<BaseInterface, RenderDeviceType, BufferDesc> {
    public:
        using BaseType = DeviceObjectBase<BaseInterface, RenderDeviceType, BufferDesc>;

        BufferBase(Pointer<RenderDeviceType> Device, const BufferDesc& Desc)
            : BaseType(Device, Desc)
            , m_DefaultUAV(nullptr)
            , m_DefaultSRV(nullptr) {

            UINT AllowedBindFlags = BIND_VERTEX_BUFFER | BIND_INDEX_BUFFER |
                                    BIND_UNIFORM_BUFFER | BIND_SHADER_RESOURCE | 
                                    BIND_STREAM_OUTPUT | BIND_UNORDERED_ACCESS | 
                                    BIND_INDIRECT_DRAW_ARGS;
            CHECK((Desc.BindFlags & ~AllowedBindFlags) == 0, 
                  "Incorrect bind flags specified (", Desc.BindFlags & ~AllowedBindFlags, ")");

            if ((BaseType::m_Desc.BindFlags & BIND_UNORDERED_ACCESS) 
                || (BaseType::m_Desc.BindFlags & BIND_SHADER_RESOURCE)) {
                
                CHECK(BaseType::m_Desc.Mode > BUFFER_MODE_UNDEFINED 
                      && BaseType::m_Desc.Mode < BUFFER_MODE_NUM_MODES,
                      "Buffer mode (", BaseType::m_Desc.Mode, ") is not correct");
                
                if (BaseType::m_Desc.Mode == BUFFER_MODE_STRUCTURED 
                    || BaseType::m_Desc.Mode == BUFFER_MODE_FORMATTED) {
                    CHECK(BaseType::m_Desc.ElementByteStride != 0, 
                          "Element stride cannot be zero for structured and formatted buffers");
                }
            }

            UINT64 DeviceQueueMask = Device->GetCommandQueueMask();
            CHECK((BaseType::m_Desc.CommandQueueMask & DeviceQueueMask) != 0, 
                  "No bits in the command queue mask (0x", std::hex, this->m_Desc.CommandQueueMask, 
                  ") correspond to one of ", pDevice->GetCommandQueueCount(), " available device command queues");
            BaseType::m_Desc.CommandQueueMask &= DeviceQueueMask;
        }

        inline virtual IBufferView* CreateView(const BufferViewDesc& ViewDesc) override;
        inline virtual IBufferView* GetDefaultView(BUFFER_VIEW_TYPE ViewType) override;
        inline virtual void SetState(RESOURCE_STATE State) override final { m_State = State; }
        inline virtual void GetState() const override final { return m_State; }
        void CreateDefaultViews(); // Create default views for structured and raw buffers. 
        inline bool CheckState(RESOURCE_STATE State) const { 
            CHECK((State & (State - 1)) == 1, "Single state is expected"); 
            CHECK(m_State != RESOURCE_STATE_UNKNOWN, "Buffer state is unknown");
            return (m_State & State) == State;
        }

    protected:
        virtual IBufferView* CreateViewInternal(const BufferViewDesc& ViewDesc, bool IsDefaultView) = 0;
        void CorrectBufferViewDesc(BufferViewDesc& ViewDesc);

    protected:
        RESOURCE_STATE m_State = RESOURCE_STATE_UNKNOWN;
        std::unique_ptr<BufferViewType> m_DefaultUAV;
        std::unique_ptr<BufferViewType> m_DefaultSRV;
    };








    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //
    // definition
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    template<typename BaseInterface, typename RenderDeviceType, typename BufferViewType>
    inline IBufferView* BufferBase<BaseInterface, RenderDeviceType, BufferViewType>::CreateView(const BufferViewDesc& ViewDesc) {
        CHECK(ViewDesc.ViewType != BUFFER_VIEW_UNDEFINED, "Buffer view type is not specified");

        if (ViewDesc.ViewType == BUFFER_VIEW_SHADER_RESOURCE)
            CHECK(BaseType::m_Desc.BindFlags & BIND_SHADER_RESOURCE, 
                  "Attempting to create SRV for buffer '", BaseType::m_Desc.Name, "' that was not created with BIND_SHADER_RESOURCE flag");
        else if (ViewDesc.ViewType == BUFFER_VIEW_UNORDERED_ACCESS)
            CHECK(BaseType::m_Desc.BindFlags & BIND_UNORDERED_ACCESS,
                  "Attempting to create UAV for buffer '", BaseType::m_Desc.Name, "' that was not created with BIND_UNORDERED_ACCESS flag");
        else
            UNEXPECTED("Unexpected buffer view type");

        return CreateViewInternal(ViewDesc, false);
    }

    template<typename BaseInterface, typename RenderDeviceType, typename BufferViewType>
    inline IBufferView* BufferBase<BaseInterface, RenderDeviceType, BufferViewType>::GetDefaultView(BUFFER_VIEW_TYPE ViewType) {
        switch (ViewType) {
            case BUFFER_VIEW_SHADER_RESOURCE: return m_DefaultSRV.get();
            case BUFFER_VIEW_UNORDERED_ACCESS: return m_DefaultUAV.get();
            default: UNEXPECTED("Unknown view type"); return nullptr;
        }
    }

    template<typename BaseInterface, typename RenderDeviceType, typename BufferViewType>
    void BufferBase<BaseInterface, RenderDeviceType, BufferViewType>::CreateDefaultViews() {
        if (BaseType::m_Desc.BindFlags & BIND_UNORDERED_ACCESS 
            && (BaseType::m_Desc.Mode == BUFFER_MODE_STRUCTURED || BaseType::m_Desc.Mode == BUFFER_MODE_RAW)) {

            BufferViewDesc ViewDesc;
            ViewDesc.ViewType = BUFFER_VIEW_UNORDERED_ACCESS;
            m_DefaultUAV.reset(static_cast<BufferViewType*>(CreateViewInternal(ViewDesc, true)));

            CHECK(m_DefaultUAV->GetDesc().ViewType == BUFFER_VIEW_UNORDERED_ACCESS, "Unexpected view type");
        }

        if (BaseType::m_Desc.BindFlags & BIND_SHADER_RESOURCE 
            && (BaseType::m_Desc.Mode == BUFFER_MODE_STRUCTURED || BaseType::m_Desc.Mode == BUFFER_MODE_RAW)) {
            
            BufferViewDesc ViewDesc;
            ViewDesc.ViewType = BUFFER_VIEW_SHADER_RESOURCE;
            m_DefaultSRV.reset(static_cast<BufferViewType*>(CreateViewInternal(ViewDesc, true)));

            CHECK(m_DefaultSRV->GetDesc().ViewType == BUFFER_VIEW_SHADER_RESOURCE, "Unexpected view type");
        }
    }


    template<typename BaseInterface, typename RenderDeviceType, typename BufferViewType>
    void BufferBase<BaseInterface, RenderDeviceType, BufferViewType>::CorrectBufferViewDesc(BufferViewDesc& ViewDesc) {
        if (ViewDesc.ByteWidth == 0) {
            CHECK(BaseType::m_Desc.SizeInBytes > ViewDesc.ByteOffset,
                  "Byte offset (", ViewDesc.ByteOffset, ") exceeds buffer size (", BaseType::m_Desc.SizeInBytes, ")");
            
            ViewDesc.ByteWidth = BaseType::m_Desc.SizeInBytes - ViewDesc.ByteOffset;
        }

        if (ViewDesc.ByteOffset + ViewDesc.ByteWidth > BaseType::m_Desc.SizeInBytes)
            LOG_AND_THROW("Buffer view range [", ViewDesc.ByteOffset, ", ", ViewDesc.ByteOffset + ViewDesc.ByteWidth,
                          ") is out of the buffer boundaries [0, ", BaseType::m_Desc.uiSizeInBytes, ").");

        if ((BaseType::m_Desc.BindFlags & BIND_UNORDERED_ACCESS) || (BaseType::m_Desc.BindFlags & BIND_SHADER_RESOURCE)) {

            if (BaseType::m_Desc.Mode == BUFFER_MODE_STRUCTURED || BaseType::m_Desc.Mode == BUFFER_MODE_FORMATTED) {
                CHECK(BaseType::m_Desc.ElementByteStride != 0, "Element byte stride is zero");

                if ((ViewDesc.ByteOffset % BaseType::m_Desc.ElementByteStride) != 0)
                    LOG_AND_THROW("Buffer view byte offset (", ViewDesc.ByteOffset, 
                                  ") is not multiple of element byte stride (", BaseType::m_Desc.ElementByteStride, ").");

                if ((ViewDesc.ByteWidth % BaseType::m_Desc.ElementByteStride) != 0)
                    LOG_AND_THROW("Buffer view byte width (", ViewDesc.ByteWidth, 
                                  ") is not multiple of element byte stride (", BaseType::m_Desc.ElementByteStride, ").");
            }

            if (BaseType::m_Desc.Mode == BUFFER_MODE_FORMATTED && ViewDesc.Format.ValueType == VT_UNDEFINED)
                LOG_AND_THROW("Format must be specified when creating a view of a formatted buffer");

            if (BaseType::m_Desc.Mode == BUFFER_MODE_FORMATTED 
                || (BaseType::m_Desc.Mode == BUFFER_MODE_RAW && ViewDesc.Format.ValueType != VT_UNDEFINED)) {

                if (ViewDesc.Format.NumComponents <= 0 || ViewDesc.Format.NumComponents > 4)
                    LOG_AND_THROW("Incorrect number of components (", UINT{ ViewDesc.Format.NumComponents }, "). 1, 2, 3, or 4 are allowed values");
                
                if (ViewDesc.Format.ValueType == VT_FLOAT32 || ViewDesc.Format.ValueType == VT_FLOAT16)
                    ViewDesc.Format.IsNormalized = false;

                auto ViewElementStride = GetValueSize(ViewDesc.Format.ValueType) * UINT{ ViewDesc.Format.NumComponents };

                if (BaseType::m_Desc.Mode == BUFFER_MODE_RAW && BaseType::m_Desc.ElementByteStride == 0)
                    LOG_AND_THROW("To enable formatted views of a raw buffer, element byte must be specified during buffer initialization");

                if (ViewElementStride != BaseType::m_Desc.ElementByteStride)
                    LOG_AND_THROW("Buffer element byte stride (", BaseType::m_Desc.ElementByteStride, 
                                  ") is not consistent with the size (", ViewElementStride, 
                                  ") defined by the format of the view (", ToStr(ViewDesc.Format), ')');
            }

            if (BaseType::m_Desc.Mode == BUFFER_MODE_RAW && ViewDesc.Format.ValueType == VT_UNDEFINED) {
                if ((ViewDesc.ByteOffset % 16) != 0)
                    LOG_AND_THROW("When creating a RAW view, the offset of the first element from the start of the buffer (", ViewDesc.ByteOffset, 
                                  ") must be a multiple of 16 bytes");
            }
        }
    }
}