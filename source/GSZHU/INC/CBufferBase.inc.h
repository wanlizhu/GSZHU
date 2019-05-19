#pragma once

#include <GSZHU/IBuffer.h>
#include <GSZHU/BasicTypes.h>
#include "CDeviceObjectBase.h"

namespace GSZHU {
    template<typename INTERFACE, typename RENDER_DEVICE>
    class CBufferBase : public CDeviceObjectBase<INTERFACE, RENDER_DEVICE> {
    public:
        using BASE = CDeviceObjectBase<INTERFACE, RENDER_DEVICE>;
        using DESC = typename INTERFACE::DESC;
        using BUFFER_VIEW = typename INTERFACE::BUFFER_VIEW;
        using VIEW_DESC = typename INTERFACE::BUFFER_VIEW::DESC;

        CBufferBase(RENDER_DEVICE* Device, const DESC& Desc);

        virtual IBufferView* CreateView(const VIEW_DESC& ViewDesc) override;
        virtual IBufferView* GetDefaultView(EBUFFER_VIEW_TYPE ViewType) override; // For formatted buffers we do not know the view format, so cannot create views.
        bool CreateDefaultViews();
        void AdjustBufferViewDesc(VIEW_DESC& ViewDesc) const;
        virtual void SetState(ERESOURCE_STATE State) override final;
        virtual ERESOURCE_STATE GetState() const override final;
        bool CheckState(ERESOURCE_STATE State) const;

    protected:
        virtual IBufferView* CreateViewInternal(const VIEW_DESC& ViewDesc, bool IsDefaultView) = 0;

    protected:
        ERESOURCE_STATE mState = RESOURCE_STATE_UNKNOWN;
        std::unique_ptr<BUFFER_VIEW> mDefaultUAV;
        std::unique_ptr<BUFFER_VIEW> mDefaultSRV;
    };
}