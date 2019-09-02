#include "gtest/gtest.h"
#include "Utils/Platform/OS.h"
#include "Utils/String.h"

using namespace GS;

TEST(Test_OS, Test_FindDataFile)
{
	auto path = OS::FindDataFile(L"icon/logo-256.ico");
	EXPECT_TRUE(path.has_value());

	std::wstring str = SZ<wchar_t>::Canonicalize(OS::GetPWD() + L"/resource/icon/logo-256.ico");
	EXPECT_STREQ(path.value().c_str(), str.c_str());
}