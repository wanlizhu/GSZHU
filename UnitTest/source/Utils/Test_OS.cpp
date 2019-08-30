#include "gtest/gtest.h"
#include "Utils/Platform/OS.h"

using namespace GS;

TEST(Test_OS, Test_FindFiles)
{
	auto path = OS::FindDataFile(L"logo-256.ico");
	int iii = 0;
}