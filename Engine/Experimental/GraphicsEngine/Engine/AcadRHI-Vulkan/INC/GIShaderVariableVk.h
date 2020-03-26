#pragma once

#include "GICommonVk.h"
#include "SPIRVReflection.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDescriptorSetVk;
    class GIDescriptorResourceVk;

    class GIShaderVariableVk
    {
    public:
        static SharedPtr<GIShaderVariableVk> Create(const SPIRVVariable& variable);

        virtual ~GIShaderVariableVk();
        bool IsValid() const;
        void BindResource(WeakPtr<GIDescriptorResourceVk> resource);
        void BindDescriptorSet(WeakPtr<GIDescriptorSetVk> set);
        
        void WriteBlockField(const char* block, const char* field, const void* data);
        void WriteRange(uint32_t offset, uint32_t size, const void* data);
        
    private:
        GIShaderVariableVk(const SPIRVVariable& variable);
        void Update();

    private:
        bool mPendingUpdate = false;
        SPIRVVariable mSPIRVVariable;
        WeakPtr<GIDescriptorSetVk> mDescriptorSet;
        WeakPtr<GIDescriptorResourceVk> mBoundResource;
    };
}