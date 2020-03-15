#pragma once

#if defined(__ANDROID__)
#include "GIAndroidPlatformVk.h"
#elif defined(_WIN32)
#include "GIWindowsPlatformVk.h"
#else
#error "!!! Undefined platform !!!"
#endif