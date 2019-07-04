#include <gtest/gtest.h>
#include "Common/String.h"

struct StringReplaceParam 
{
    std::string str;
    std::string token;
    std::string replacer;
    std::string result;
};

template<>
std::string testing::PrintToString(const StringReplaceParam& param)
{
    return "(" + param.str + " " + param.token + "->" + param.replacer + ")";
}

class StringReplaceTest : public testing::Test
                          , public testing::WithParamInterface<StringReplaceParam>
{
public:
    virtual void SetUp() override 
    {}

    virtual void TearDown() override
    {}
};

TEST_P(StringReplaceTest, Case1)
{
    const StringReplaceParam& param = GetParam();

    auto real = ZHU::String::Replace(param.str, param.token, param.replacer);
    auto should = param.result;
    EXPECT_EQ(real, should);
}

INSTANTIATE_TEST_CASE_P(Instantiation,
                        StringReplaceTest,
                        ::testing::Values(
                            StringReplaceParam{ "this*is*a*test", "*", "XX", "thisXXisXXaXXtest" },
                            StringReplaceParam{ "*this*is*a**test*", "*", "&&", "&&this&&is&&a&&&&test&&" },
                            StringReplaceParam{ "234.36545", ".36", "YY", "234YY545" },
                            StringReplaceParam{ "234e-12", "-12", "", "234e" },
                            StringReplaceParam{ ".f", ".", "", "f" },
                            StringReplaceParam{ "0", "0", "", "" },
                            StringReplaceParam{ "-0", "-", "THIS", "THIS0" },
                            StringReplaceParam{ "-", "-", "this is a", "this is a" },
                            StringReplaceParam{ "987234", "987", "", "234" }
));