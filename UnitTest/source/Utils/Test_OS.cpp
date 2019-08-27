#include "gtest/gtest.h"
#include "Utils/Platform/OS.h"

using namespace GS;

TEST(Test_OS, Test_FindFiles)
{
	auto vec = OS::FindFiles("logo.ico", "d:/GraphicsEngine/Engine/resource/icon");

}