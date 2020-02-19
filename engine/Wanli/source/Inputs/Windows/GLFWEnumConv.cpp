#include "Inputs/Windows/GLFWEnumConv.h"

namespace Wanli
{
	EKey GlfwToEKey(int key)
	{
		return static_cast<EKey>(key);
	}

	EMouseButton GlfwToEMouseButton(int button)
	{
		return static_cast<EMouseButton>(button);
	}

	EInputAction GlfwToEInputAction(int action)
	{
		return static_cast<EInputAction>(action);
	}

	EInputMods GlfwToEInputMods(int mods)
	{
		return static_cast<EInputMods>(mods);
	}
}