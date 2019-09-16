#pragma once

#ifdef RHI_USE_OPENGL
#include "OpenGL/ConfigOpenGL.h"
#elif defined RHI_USE_VULKAN
#include "Vulkan/ConfigVulkan.h"
#elif defined RHI_USE_D3D12
#include "D3D12/ConfigD3D12.h"
#endif
