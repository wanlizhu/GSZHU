#pragma once

#include <GSZHU/IPipelineState.h>
#include <GSZHU/SLayoutElement.h>
#include <GSZHU/Constants.h>
#include <GSZHU/BasicTypes.h>
#include "CDeviceObjectBase.h"

namespace GSZHU {
    template<typename INTERFACE, typename RENDER_DEVICE>
    class CPipelineStateBase : public CDeviceObjectBase<INTERFACE, RENDER_DEVICE> {
    public:
        using BASE = CDeviceObjectBase<INTERFACE, RENDER_DEVICE>;
        using DESC = typename INTERFACE::DESC;

        CPipelineStateBase(RENDER_DEVICE* Device, const DESC& Desc);
        virtual ~CPipelineStateBase();

        virtual const uint32_t* GetBufferStrides() const;
        uint32_t GetNumBufferSlots() const;
        size_t GetShaderResourceLayoutHash() const;
        virtual void BindShaderResources(IResourceTable* ResourceTable, uint32_t Flags) override;
        IShader* const* GetShaders() const;
        uint32_t GetNumShaders() const;
        IShader* GetShaderVS() const;
        IShader* GetShaderPS() const;
        IShader* GetShaderGS() const;
        IShader* GetShaderDS() const;
        IShader* GetShaderHS() const;
        IShader* GetShaderCS() const;

    protected:
        std::vector<SLayoutElement> mLayoutElements;
        std::array<uint32_t, MAX_BUFFER_SLOTS> mBufferStrides = {};
        uint32_t mNumBufferSlots = 0;

        std::vector<IShader*> mShaders;
        size_t mShaderResourceLayoutHash = 0;
    };
}