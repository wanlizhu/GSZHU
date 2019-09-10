#pragma once

#include "ResourceFormat.h"

#ifdef RHI_USE_OPENGL
#include "LowLevel/OpenGL/ConfigOpenGL.h"
#elif defined RHI_USE_VULKAN
#include "LowLevel/Vulkan/ConfigVulkan.h"
#elif defined RHI_USE_D3D12
#include "LowLevel/D3D12/ConfigD3D12.h"
#endif
