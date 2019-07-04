#include <gtest/gtest.h>
#include "Common/String.h"

template<typename T>
struct StringFromParam 
{
    T value;
    std::string result;
};

template<>
std::string testing::PrintToString(const StringFromParam<long>& param)
{
    return std::to_string(param.value);
}

template<>
std::string testing::PrintToString(const StringFromParam<double>& param)
{
    return std::to_string(param.value);
}

class StringFromLongTest : public testing::Test
                         , public testing::WithParamInterface<StringFromParam<long>>
{
public:
    virtual void SetUp() override 
    {}

    virtual void TearDown() override
    {}
};

class StringFromDoubleTest : public testing::Test
                           , public testing::WithParamInterface<StringFromParam<double>>
{
public:
    virtual void SetUp() override
    {}

    virtual void TearDown() override
    {}
};

TEST_P(StringFromLongTest, Case1)
{
    const StringFromParam<long>& param = GetParam();

    std::string real = ZHU::String::From<long>(param.value);
    std::string should = param.result;
    EXPECT_EQ(real, should);
}

TEST_P(StringFromDoubleTest, Case1)
{
    const StringFromParam<double>& param = GetParam();

    std::string real = ZHU::String::From<double>(param.value);
    std::string should = param.result;
    EXPECT_EQ(real, should);
}


INSTANTIATE_TEST_CASE_P(Instantiation,
                        StringFromLongTest,
                        ::testing::Values(
                            StringFromParam<long>{ 24234, "24234" },
                            StringFromParam<long>{ -24234, "-24234" },
                            StringFromParam<long>{ 234, "234" },
                            StringFromParam<long>{ 76587, "76587" },
                            StringFromParam<long>{ 4536, "4536" },
                            StringFromParam<long>{ 234, "234" },
                            StringFromParam<long>{ 4, "4" },
                            StringFromParam<long>{ 45, "45" },
                            StringFromParam<long>{ -12342, "-12342" },
                            StringFromParam<long>{ 2, "2" },
                            StringFromParam<long>{ 35, "35" },
                            StringFromParam<long>{ -25, "-25" },
                            StringFromParam<long>{ 234, "234" },
                            StringFromParam<long>{ -23467, "-23467" },
                            StringFromParam<long>{ 23, "23" },
                            StringFromParam<long>{ 0, "0" },
                            StringFromParam<long>{ -0, "0" }
));


INSTANTIATE_TEST_CASE_P(Instantiation,
                        StringFromDoubleTest,
                        ::testing::Values(
                            StringFromParam<double>{ 24.234, "24.234000" },
                            StringFromParam<double>{ -2.4234, "-2.423400" },
                            StringFromParam<double>{ 234., "234.000000" },
                            StringFromParam<double>{ .76587, "0.765870" },
                            StringFromParam<double>{ 45.36, "45.360000" },
                            StringFromParam<double>{ .234, "0.234000" },
                            StringFromParam<double>{ 4., "4.000000" },
                            StringFromParam<double>{ 4.5, "4.500000" },
                            StringFromParam<double>{ -12.342, "-12.342000" },
                            StringFromParam<double>{ .2, "0.200000" },
                            StringFromParam<double>{ 3.5, "3.500000" },
                            StringFromParam<double>{ -2.5, "-2.500000" },
                            StringFromParam<double>{ 2.34, "2.340000" },
                            StringFromParam<double>{ -.23467, "-0.234670" },
                            StringFromParam<double>{ 2.3, "2.300000" },
                            StringFromParam<double>{ 0, "0.000000" },
                            StringFromParam<double>{ -0, "0.000000" }
));