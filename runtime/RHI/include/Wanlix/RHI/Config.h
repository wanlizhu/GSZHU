#pragma once

#if defined _MSC_VER && !defined WANLIX_RHI_BUILD_SHARED_LIBS
#   define RHI_API __declspec(dllexport)
#else
#   define RHI_API
#endif

#define RHI_MAX_NUM_COLOR_ATTACHMENTS   8
#define RHI_MAX_NUM_ATTACHMENTS         (RHI_MAX_NUM_COLOR_ATTACHMENTS + 1)
#define RHI_MAX_NUM_VIEWPORTS           16
#define RHI_MAX_NUM_SCISSORS            RHI_MAX_NUM_VIEWPORTS
#define RHI_MAX_NUM_SAMPLES             64
#define RHI_MAX_NUM_STREAMOUT_BUFFERS   4