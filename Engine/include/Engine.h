#pragma once

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "EngineConfig.h"
#include "Framework.h"
#include "ApplicationBase.h"
#include "IRenderer.h"
#include "ScriptBindings.h"

#define _USE_MATH_DEFINES
#include <math.h>

// Core
#include "RHI/Object.h"
#include "RHI/IDeviceObject.h"
#include "RHI/IResource.h"
#include "RHI/IBuffer.h"

