#include "gtest/gtest.h"
#include "Utils/System.h"
#include "Utils/String.h"

using namespace GS;

TEST(Test_OS, Test_FindDataFile)
{
	auto path = FindDataFile(L"icon/logo-128.ico");
	EXPECT_TRUE(!path.empty());

	std::wstring str = Canonicalize(STR(CurrentDirectory()) + L"/resource/icon/logo-128.ico");
	EXPECT_STREQ(path.c_str(), str.c_str());
}