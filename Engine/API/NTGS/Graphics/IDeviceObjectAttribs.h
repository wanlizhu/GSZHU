#pragma once

#include <NTGS/Core/Config.h>
#include <NTGS/Graphics/GraphicsTypes.h>
#include <NTGS/Graphics/Utilities.h>

namespace NTGS {
    class ITexture;
    class IBuffer;

    // Describes common device object attributes
    struct IDeviceObjectAttribs {
        const char* Name = nullptr;

        IDeviceObjectAttribs() = default;
        explicit IDeviceObjectAttribs(const char* _Name) : Name(_Name) {}
    };

    struct HardwareAdapterAttribs {
        char Description[128] = { 0 };
        SIZE DedicatedVideoMemory = 0;
        SIZE DedicatedSystemMemory = 0;
        SIZE SharedSystemMemory = 0;

        UINT VendorID = 0;
        UINT DeviceID = 0;
        UINT NumOutputs = 0;
    };

    struct D3D12Attribs {
        static constexpr UINT DefaultAdapterID = 0xFFFFFFFF;

        UINT AdapterID = DefaultAdapterID;
        UINT CPUDescriptorHeapAllocationSize[4] = {
            8192,  // D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
            2048,  // D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER
            1024,  // D3D12_DESCRIPTOR_HEAP_TYPE_RTV
            1024   // D3D12_DESCRIPTOR_HEAP_TYPE_DSV
        };
        UINT GPUDescriptorHeapSize[2] = {
            16384, // D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
            1024   // D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER
        };
        UINT GPUDescriptorHeapDynamicSize[2] = {
            8192,  // D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
            1024   // D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER
        };
        UINT DynamicDescriptorAllocationChunkSize[2] = {
            256,  // D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
            32    // D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER
        };
        UINT NumCommandsToFlushCommandList = 256;
        UINT DynamicHeapPageSize = 1 << 20;
        UINT NumDynamicHeapPagesToReserve = 1;
    };

    struct VulkanAttribs {
        struct DeviceFeatures {
            bool DepthBiasClamp = false;
            bool FillModeNonSolid = false;
            bool DepthClamp = false;
            bool IndependentBlend = false;
            bool SamplerAnisotropy = false;
            bool GeometryShader = false;
            bool TessellationShader = false;
            bool DualSrcBlend = false;
            bool MultiViewport = false;
            bool ImageCubeArray = false;
            bool TextureCompressionBC = false;
            bool VertexPipelineStoresAndAtomics = false;
            bool FragmentStoresAndAtomics = false;
            bool ShaderStorageImageExtendedFormats = false;
        };

        struct DescriptorPoolSize {
            UINT MaxDescriptorSets = 0;
            UINT NumSeparateSamplerDescriptors = 0;
            UINT NumCombinedSamplerDescriptors = 0;
            UINT NumSampledImageDescriptors = 0;
            UINT NumStorageImageDescriptors = 0;
            UINT NumUniformBufferDescriptors = 0;
            UINT NumStorageBufferDescriptors = 0;
            UINT NumUniformTexelBufferDescriptors = 0;
            UINT NumStorageTexelBufferDescriptors = 0;

            DescriptorPoolSize() = default;

            DescriptorPoolSize(UINT _MaxDescriptorSets,
                UINT _NumSeparateSamplerDescriptors,
                UINT _NumCombinedSamplerDescriptors,
                UINT _NumSampledImageDescriptors,
                UINT _NumStorageImageDescriptors,
                UINT _NumUniformBufferDescriptors,
                UINT _NumStorageBufferDescriptors,
                UINT _NumUniformTexelBufferDescriptors,
                UINT _NumStorageTexelBufferDescriptors) noexcept 
                : MaxDescriptorSets(_MaxDescriptorSets),
                  NumSeparateSamplerDescriptors(_NumSeparateSamplerDescriptors),
                  NumCombinedSamplerDescriptors(_NumCombinedSamplerDescriptors),
                  NumSampledImageDescriptors(_NumSampledImageDescriptors),
                  NumStorageImageDescriptors(_NumStorageImageDescriptors),
                  NumUniformBufferDescriptors(_NumUniformBufferDescriptors),
                  NumStorageBufferDescriptors(_NumStorageBufferDescriptors),
                  NumUniformTexelBufferDescriptors(_NumUniformTexelBufferDescriptors),
                  NumStorageTexelBufferDescriptors(_NumStorageTexelBufferDescriptors) {}
        };

        bool EnableValidation = false;
        UINT GlobalExtensionCount = 0;
        const char* const* GlobalExtensionNames = nullptr;
        void* Allocator = nullptr;
        UINT NumCommandsToFlushCommandBuffer = 256;

        DeviceFeatures EnabledFeatures;
        //                                         Max  SepSm  CmbSm  SmpImg StrImg   UB     SB    UTxB   StTxB
        DescriptorPoolSize MainDescriptorPoolSize{ 8192,  1024,  8192,  8192,  1024,  4096,  4096,  1024,  1024 };
        //                                            Max  SepSm  CmbSm  SmpImg StrImg   UB     SB    UTxB   StTxB
        DescriptorPoolSize DynamicDescriptorPoolSize{ 2048,   256,  2048,  2048,   256,  1024,  1024,   256,   256 };

        UINT DeviceLocalMemoryPageSize = 16 << 20;
        UINT HostVisibleMemoryPageSize = 16 << 20;
        UINT DeviceLocalMemoryReserveSize = 256 << 20;
        UINT HostVisibleMemoryReserveSize = 256 << 20;
        UINT UploadHeapPageSize = 1 << 20;
        UINT DynamicHeapSize = 8 << 20;
        UINT DynamicHeapPageSize = 256 << 10;
    };

    template<typename BackendType>
    struct EngineCreationAttribs : public BackendType {
        class IMemoryAllocator* MemoryAllocator = nullptr;
        DebugOutput::MessageCallback DebugMessageCallback = nullptr;
    };
}