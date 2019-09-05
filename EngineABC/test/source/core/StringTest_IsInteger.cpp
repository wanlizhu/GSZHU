#include <gtest/gtest.h>
#include "Common/Utilities/String.h"

struct StringIsIntegerParam 
{
    std::string str;
    bool result;
};

template<>
std::string testing::PrintToString(const StringIsIntegerParam& param)
{
    return param.str;
}

class StringIsIntegerTest : public testing::Test
                          , public testing::WithParamInterface<StringIsIntegerParam>
{
public:
    virtual void SetUp() override 
    {}

    virtual void TearDown() override
    {}
};

TEST_P(StringIsIntegerTest, Case1)
{
    const StringIsIntegerParam& param = GetParam();

    auto real = ZHU::String::IsInteger(param.str);
    auto should = param.result;
    EXPECT_EQ(real, should);
}

INSTANTIATE_TEST_CASE_P(Instantiation,
                        StringIsIntegerTest,
                        ::testing::Values(
                            StringIsIntegerParam{ "this*is*a*test", false },
                            StringIsIntegerParam{ "adfgdf", false },
                            StringIsIntegerParam{ "234.36545", false },
                            StringIsIntegerParam{ "234e-12", false },
                            StringIsIntegerParam{ ".f", false },
                            StringIsIntegerParam{ "0", true },
                            StringIsIntegerParam{ "-0", true },
                            StringIsIntegerParam{ "-", false },
                            StringIsIntegerParam{ "987234", true },
                            StringIsIntegerParam{ "29347762", true },
                            StringIsIntegerParam{ "-9824", true },
                            StringIsIntegerParam{ "0.f", false }
));