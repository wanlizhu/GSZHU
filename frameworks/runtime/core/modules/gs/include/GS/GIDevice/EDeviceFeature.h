#pragma once

#include <stdint.h>

namespace GS2::GI
{
    enum class EDeviceFeature : uint64_t
    {
        None = 0,
        RobustBufferAccess                          = 1,
        FullDrawIndexUint32                         = 1 << 1,
        ImageCubeArray                              = 1 << 2,
        IndependentBlend                            = 1 << 3,
        GeometryShader                              = 1 << 4,
        TessellationShader                          = 1 << 5,
        SampleRateShading                           = 1 << 6,
        DualSrcBlend                                = 1 << 7,
        LogicOP                                     = 1 << 8,
        MultiDrawIndirect                           = 1 << 9,
        DrawIndirectFirstInstance                   = 1 << 10,
        DepthClamp                                  = 1 << 11,
        DepthBiasClamp                              = 1 << 12,
        FillModeNoSolid                             = 1 << 13,
        DepthBounds                                 = 1 << 14,
        WideLines                                   = 1 << 15,
        LargePoints                                 = 1 << 16,
        AlphaToOne                                  = 1 << 17,
        MultiViewport                               = 1 << 18,
        SamplerAnisotropy                           = 1 << 19,
        TextureCompressionETC2                      = 1 << 20,
        TextureCompressionASTC_LDR                  = 1 << 21,
        TextureCompressionBC                        = 1 << 22,
        OcclusionQueryPrecise                       = 1 << 23,
        PipelineStatisticsQuery                     = 1 << 24,
        VertexPipelineStoresAndAtomics              = 1 << 25,
        FragmentPipelineStoresAndAtomics            = 1 << 26,
        ShaderTessellationAndGeometryPointSize      = 1 << 27,
        ShaderImageGatherExtended                   = 1 << 28,
        ShaderStorageImageExtendedFormats           = 1 << 29,
        ShaderStorageImageMultisample               = 1 << 30,
        ShaderStorageImageReadWithoutFormat         = 1 << 31,
        ShaderStorageImageWriteWithoutFormat        = 1 << 32,
        ShaderUniformBufferArrayDynamicIndexing     = 1 << 33,
        ShaderSampledImageArrayDynamicIndexing      = 1 << 34,
        ShaderStorageBufferArrayDynamicIndexing     = 1 << 35,
        ShaderStorageImageArrayDynamicIndexing      = 1 << 36,
        ShaderClipDistance                          = 1 << 37,
        ShaderCullDistance                          = 1 << 38,
        ShaderFloat64                               = 1 << 39,
        ShaderInt64                                 = 1 << 40,
        ShaderInt16                                 = 1 << 41,
        ShaderResourceResidency                     = 1 << 42,
        ShaderResourceMinLOD                        = 1 << 43,
        SparseResidencyBuffer                       = 1 << 44,
        SparseResidencyImage2D                      = 1 << 45,
        SparseResidencyImage3D                      = 1 << 46,
        SparseResidency2Samples                     = 1 << 47,
        SparseResidency4Samples                     = 1 << 48,
        SparseResidency8Samples                     = 1 << 49,
        SparseResidency16Samples                    = 1 << 50,
        SparseResidencyAliased                      = 1 << 51,
        VariableMultisampleRate                     = 1 << 52,
        InheritedQueries                            = 1 << 53,
    };

    template<typename T>
    T fromDeviceFeatures(EDeviceFeature features);

    template<typename T>
    EDeviceFeature toDeviceFeatures(const T& native);
}