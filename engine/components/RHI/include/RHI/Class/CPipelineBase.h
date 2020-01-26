#pragma once

#include "RHI/IPipeline.h"
#include "CDeviceObjectBase.h"

namespace Wanlix
{
    class CPipelineBase : public CDeviceObjectBase<IPipeline>
    {
    public:
        inline Uint GetBufferStride(Uint slot) const { return ((Uint64)slot) < mBufferStrides.size() ? mBufferStrides.at(slot) : 0; }
        inline Uint GetNumBufferSlots() const { return (Uint)mBufferStrides.size(); }
        inline Bool IsCompatibleWith(const IPipeline* pso) const { return mResourceLayoutHash == static_cast<const CPipelineBase*>(pso)->mResourceLayoutHash; }

        inline IShader* GetVertexShader() const { return mVertexShader.get(); }
        inline IShader* GetTessCtrlShader() const { return mVertexShader.get(); }
        inline IShader* GetTessEvalShader() const { return mVertexShader.get(); }
        inline IShader* GetGeometryShader() const { return mVertexShader.get(); }
        inline IShader* GetFragmentShader() const { return mVertexShader.get(); }
        inline IShader* GetComputeShader() const { return mVertexShader.get(); }
        inline IShader* GetShader(EShaderType type) const
        {
            switch (type)
            {
                case EShaderType::Vertex: return GetVertexShader(); break;
                case EShaderType::TessCtrl: return GetTessCtrlShader(); break;
                case EShaderType::TessEval: return GetTessEvalShader(); break;
                case EShaderType::Geometry: return GetGeometryShader(); break;
                case EShaderType::Fragment: return GetFragmentShader(); break;
                case EShaderType::Compute: return GetComputeShader(); break;
                default: assert(false);
            }
        }

    protected:
        CPipelineBase(IDevice* device,
                      const PipelineDesc& desc,
                      const String& name);

    protected:
        Uint64 mResourceLayoutHash = 0;
        Array<Uint> mBufferStrides;

        SharedPtr<IShader> mVertexShader;
        SharedPtr<IShader> mTessCtrlShader;
        SharedPtr<IShader> mTessEvalShader;
        SharedPtr<IShader> mGeometryShader;
        SharedPtr<IShader> mFragmentShader;
        SharedPtr<IShader> mComputeShader;
    };
}