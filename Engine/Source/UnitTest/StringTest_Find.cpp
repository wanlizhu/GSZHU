#include <gtest/gtest.h>
#include "Core/String.h"

struct StringFindParam 
{
    std::string str;
    std::string pat;
    bool result;
};

template<>
std::string testing::PrintToString(const StringFindParam& param)
{
    return "(" + param.str + ", " + param.pat + ")";
}

class StringFindTest : public testing::Test
                     , public testing::WithParamInterface<StringFindParam>
{
public:
    virtual void SetUp() override 
    {}

    virtual void TearDown() override
    {}
};


class StringStartsWith : public StringFindTest 
{};

TEST_P(StringStartsWith, Case1)
{
    const StringFindParam& param = GetParam();
    auto real = ZHU::String::StartsWith(param.str, param.pat);
    auto should = param.result;
    EXPECT_EQ(real, should);
}

INSTANTIATE_TEST_CASE_P(Instantiation,
                        StringStartsWith,
                        ::testing::Values(
                            StringFindParam{ "this*is*a*test", "this*", true },
                            StringFindParam{ "this*is*a*test*", "t", true },
                            StringFindParam{ "*this*is*a**test**", "*this", true },
                            StringFindParam{ "\\\\this", "\\\\", true },
                            StringFindParam{ "", "", true },
                            StringFindParam{ "??", "??", true },
                            StringFindParam{ "***", "*", true }
));


class StringContains : public StringFindTest
{};

TEST_P(StringContains, Case1)
{
    const StringFindParam& param = GetParam();
    auto real = ZHU::String::Contains(param.str, param.pat);
    auto should = param.result;
    EXPECT_EQ(real, should);
}

INSTANTIATE_TEST_CASE_P(Instantiation,
                        StringContains,
                        ::testing::Values(
                            StringFindParam{ "this*is*a*test", "this*", true },
                            StringFindParam{ "this*is*a*test*", "*is*", true },
                            StringFindParam{ "*this*is*a**test**", "**", true },
                            StringFindParam{ "\\\\this", "\\\\", true },
                            StringFindParam{ "", "", true },
                            StringFindParam{ "??", "??", true },
                            StringFindParam{ "??", "???", false },
                            StringFindParam{ "***", "*", true }
));


class StringEndsWith : public StringFindTest
{};

TEST_P(StringEndsWith, Case1)
{
    const StringFindParam& param = GetParam();
    auto real = ZHU::String::EndsWith(param.str, param.pat);
    auto should = param.result;
    EXPECT_EQ(real, should);
}

INSTANTIATE_TEST_CASE_P(Instantiation,
                        StringEndsWith,
                        ::testing::Values(
                            StringFindParam{ "this*is*a*test", "this*", false },
                            StringFindParam{ "this*is*a*test*", "*is*", false },
                            StringFindParam{ "*this*is*a**test**", "**", true },
                            StringFindParam{ "\\\\this", "\\\\", false },
                            StringFindParam{ "", "", true },
                            StringFindParam{ "??", "??", true },
                            StringFindParam{ "??", "???", false },
                            StringFindParam{ "??\\?", "\\?", true },
                            StringFindParam{ "***", "*", true }
));