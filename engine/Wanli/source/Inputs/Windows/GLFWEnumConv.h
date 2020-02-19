#pragma once

#include "Core/BasicTypes.h"
#include "Inputs/InputFlags.h"

namespace Wanli
{
	EKey GlfwToEKey(int key);
	EMouseButton GlfwToEMouseButton(int button);
	EInputAction GlfwToEInputAction(int action);
	EInputMods GlfwToEInputMods(int mods);
}