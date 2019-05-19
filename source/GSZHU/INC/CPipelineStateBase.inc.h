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
        IShader* const* GetShaders() const;
        uint32_t GetNumShaders() const;
        size_t GetShaderResourceLayoutHash() const;
        virtual void BindShaderResources(IResourceTable* ResourceTable, uint32_t Flags) override;
        template<typename Class, uint32_t Type> IShader* GetShader() const {
            auto Iter = std::find_if(mShaders.begin(), mShaders.end(), [&](const IShader*& Item) {
                return Item->GetDesc().ShaderType == Type;
            });
            return dynamic_cast<Class>(Iter ? *Iter : nullptr);
        }

    protected:
        std::vector<SLayoutElement> mLayoutElements;
        std::array<uint32_t, MAX_BUFFER_SLOTS> mBufferStrides = {};
        uint32_t mNumBufferSlots = 0;

        std::vector<IShader*> mShaders;
        size_t mShaderResourceLayoutHash = 0;
    };
}