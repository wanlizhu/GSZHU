#include "GIShaderVariableVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GIShaderVariableVk> GIShaderVariableVk::Create(const SPIRVVariable& variable)
    {
        return SharedPtr<GIShaderVariableVk>(new GIShaderVariableVk(variable));
    }

    GIShaderVariableVk::GIShaderVariableVk(const SPIRVVariable& variable)
        : mSPIRVVariable(variable)
    {}

    GIShaderVariableVk::~GIShaderVariableVk()
    {}

    bool GIShaderVariableVk::IsValid() const
    {
        return !mDescriptorSet.expired() && !mBoundResource.expired();
    }

    void GIShaderVariableVk::BindResource(WeakPtr<GIDescriptorResourceVk> resource)
    {
        mBoundResource = resource;
    }

    void GIShaderVariableVk::BindDescriptorSet(WeakPtr<GIDescriptorSetVk> set)
    {
        mDescriptorSet = set;
    }

    void GIShaderVariableVk::WriteBlockField(const char* block, const char* field, const void* data)
    {
        //TODO
    }

    void GIShaderVariableVk::WriteRange(uint32_t offset, uint32_t size, const void* data)
    {
    }
    
    void GIShaderVariableVk::Update()
    {}
}