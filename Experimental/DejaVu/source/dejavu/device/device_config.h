#pragma once

#include "vulkan/vulkan_config.h"

#if defined(_WIN32) && defined(DJV_ENABLE_D3D12)
#include "d3d12/d3d12_config.h"
#endif

#if defined(__APPLE__) && defined(DJV_ENABLE_METAL)
#include "metal/metal_config.h"
#endif