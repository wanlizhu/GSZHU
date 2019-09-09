#pragma once

#include "ResourceFormat.h"

#ifdef RHI_USE_OPENGL
#include "LowLevel/ConfigOpenGL.h"
#elif defined RHI_USE_VULKAN
#include "LowLevel/ConfigVulkan.h"
#elif defined RHI_USE_D3D12
#include "LowLevel/ConfigD3D12.h"
#endif
