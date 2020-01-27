#pragma once

#include "RHI/IPipeline.h"
#include "CDeviceObjectBase.h"

namespace Wanlix
{
    template<typename _Interface_>
    class CPipelineBase : public CDeviceObjectBase<_Interface_>
    {
        static_assert(std::is_base_of_v<IPipeline, _Interface_> || std::is_same_v<IPipeline, _Interface_>);
    public:
        using Shader = typename _Interface_::Shader;

        inline Uint GetBufferStride(Uint slot) const { return ((Uint64)slot) < mBufferStrides.size() ? mBufferStrides.at(slot) : 0; }
        inline Uint GetNumBufferSlots() const { return (Uint)mBufferStrides.size(); }
        inline Bool IsCompatibleWith(const _Interface_* pso) const { return mResourceLayoutHash == static_cast<const CPipelineBase*>(pso)->mResourceLayoutHash; }

        inline Shader* GetVertexShader() const { return mVertexShader.get(); }
        inline Shader* GetTessCtrlShader() const { return mVertexShader.get(); }
        inline Shader* GetTessEvalShader() const { return mVertexShader.get(); }
        inline Shader* GetGeometryShader() const { return mVertexShader.get(); }
        inline Shader* GetFragmentShader() const { return mVertexShader.get(); }
        inline Shader* GetComputeShader() const { return mVertexShader.get(); }
        inline Shader* GetShader(EShaderType type) const
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

        SharedPtr<Shader> mVertexShader;
        SharedPtr<Shader> mTessCtrlShader;
        SharedPtr<Shader> mTessEvalShader;
        SharedPtr<Shader> mGeometryShader;
        SharedPtr<Shader> mFragmentShader;
        SharedPtr<Shader> mComputeShader;
    };
}