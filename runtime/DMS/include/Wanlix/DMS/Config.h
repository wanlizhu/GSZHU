#pragma once

#include "Wanlix/Config.h"

#if defined _MSC_VER && !defined WANLIX_RHI_BUILD_SHARED_LIBS
#   define DMS_API __declspec(dllexport)
#else
#   define DMS_API
#endif

#define DMS_MAX_VERTICES  0x7fffffff
#define DMS_MAX_FACES     0x7fffffff
#define DMS_MAX_VERTEX_TEXCOORDS 0x8
#define DMS_MAX_VERTEX_COLORS    0x8

