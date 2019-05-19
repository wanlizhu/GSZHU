#pragma once

#include "CPipelineStateBase.inc.h"

namespace GSZHU {
    template<typename INTERFACE, typename RENDER_DEVICE>
    CPipelineStateBase<INTERFACE, RENDER_DEVICE>::CPipelineStateBase(RENDER_DEVICE* Device, const DESC& Desc)
        : BASE(Device, Desc)
        , mLayoutElements(Desc.GraphicsPipeline.InputLayout.NumElements, SLayoutElement())
        , mNumShaders(0) 
        , mShaderResourceLayoutHash(0) {
        if (mDesc.IsComputePipeline) {
            InitializeComputePipeline(Device, Desc);
        }
        else {
            InitializeGraphicsPipeline(Device, Desc);
        }
    }

    template<typename INTERFACE, typename RENDER_DEVICE>
    CPipelineStateBase<INTERFACE, RENDER_DEVICE>::~CPipelineStateBase() {

    }

    template<typename INTERFACE, typename RENDER_DEVICE>
    const uint32_t* CPipelineStateBase<INTERFACE, RENDER_DEVICE>::GetBufferStrides() const {
        return mBufferStrides.data();
    }

    template<typename INTERFACE, typename RENDER_DEVICE>
    uint32_t CPipelineStateBase<INTERFACE, RENDER_DEVICE>::GetNumBufferSlots() const {
        return mNumBufferSlots;
    }

    template<typename INTERFACE, typename RENDER_DEVICE>
    IShader* const* CPipelineStateBase<INTERFACE, RENDER_DEVICE>::GetShaders() const {
        return mShaders.data();
    }

    template<typename INTERFACE, typename RENDER_DEVICE>
    uint32_t CPipelineStateBase<INTERFACE, RENDER_DEVICE>::GetNumShaders() const {
        return (uint32_t)mShaders.size();
    }

    template<typename INTERFACE, typename RENDER_DEVICE>
    size_t CPipelineStateBase<INTERFACE, RENDER_DEVICE>::GetShaderResourceLayoutHash() const {
        return mShaderResourceLayoutHash;
    }

    template<typename INTERFACE, typename RENDER_DEVICE>
    void CPipelineStateBase<INTERFACE, RENDER_DEVICE>::BindShaderResources(IResourceTable* ResourceTable, uint32_t Flags) {
        for (auto Shader : mShaders) {
            Shader->BindResources(ResourceTable, Flags);
        }
    }
}