#pragma once

#ifdef _DEBUG
#define LOG_ENABLED 1
#else
#define LOG_ENABLED 0
#endif

#define PROFILING_ENABLED 1
#define PROFILING_LOG 0
#define PROFILING_LOG_BATCH_SIZE 1024

#define NVAPI_ENABLED 1
#define GS_USE_PYTHON 1
#define GS_USE_D3D12
//#define GS_USE_VULKAN
//#define GS_USE_OPENGL 