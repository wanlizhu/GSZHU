#include <gtest/gtest.h>
#include "Common/String.h"

template<typename T>
struct StringToParam
{
    T result;
    std::string str;
};

template<>
std::string testing::PrintToString(const StringToParam<long>& param)
{
    return param.str;
}

template<>
std::string testing::PrintToString(const StringToParam<double>& param)
{
    return param.str;
}

class StringToLongTest : public testing::Test
                       , public testing::WithParamInterface<StringToParam<long>>
{
public:
    virtual void SetUp() override
    {}

    virtual void TearDown() override
    {}
};

class StringToDoubleTest : public testing::Test
                         , public testing::WithParamInterface<StringToParam<double>>
{
public:
    virtual void SetUp() override
    {}

    virtual void TearDown() override
    {}
};

TEST_P(StringToLongTest, Case1)
{
    const StringToParam<long>& param = GetParam();

    long real = ZHU::String::To<long>(param.str);
    long should = param.result;
    EXPECT_EQ(real, should);
}

TEST_P(StringToDoubleTest, Case1)
{
    const StringToParam<double>& param = GetParam();

    double real = ZHU::String::To<double>(param.str);
    double should = param.result;
    EXPECT_FLOAT_EQ((float)real, (float)should);
}


INSTANTIATE_TEST_CASE_P(Instantiation,
                        StringToLongTest,
                        ::testing::Values(
                            StringToParam<long>{ 24234, "24234" },
                            StringToParam<long>{ -24234, "-24234" },
                            StringToParam<long>{ 234, "234" },
                            StringToParam<long>{ 76587, "76587" },
                            StringToParam<long>{ 4536, "4536" },
                            StringToParam<long>{ 234, "234" },
                            StringToParam<long>{ 4, "4" },
                            StringToParam<long>{ 45, "45" },
                            StringToParam<long>{ -12342, "-12342" },
                            StringToParam<long>{ 2, "2" },
                            StringToParam<long>{ 35, "35" },
                            StringToParam<long>{ -25, "-25" },
                            StringToParam<long>{ 234, "234" },
                            StringToParam<long>{ -23467, "-23467" },
                            StringToParam<long>{ 23, "23" },
                            StringToParam<long>{ 0, "0" },
                            StringToParam<long>{ -0, "0" }
));


INSTANTIATE_TEST_CASE_P(Instantiation,
                        StringToDoubleTest,
                        ::testing::Values(
                            StringToParam<double>{ 24.234, "24.234000" },
                            StringToParam<double>{ -2.4234, "-2.423400" },
                            StringToParam<double>{ 234., "234.000000" },
                            StringToParam<double>{ .76587, "0.765870" },
                            StringToParam<double>{ 45.36, "45.360000" },
                            StringToParam<double>{ .234, "0.234000" },
                            StringToParam<double>{ 4., "4.000000" },
                            StringToParam<double>{ 4.5, "4.500000" },
                            StringToParam<double>{ -12.342, "-12.342000" },
                            StringToParam<double>{ .2, "0.200000" },
                            StringToParam<double>{ 3.5, "3.500000" },
                            StringToParam<double>{ -2.5, "-2.500000" },
                            StringToParam<double>{ 2.34, "2.340000" },
                            StringToParam<double>{ -.23467, "-0.234670" },
                            StringToParam<double>{ 2.3, "2.300000" },
                            StringToParam<double>{ 0, "0.000000" },
                            StringToParam<double>{ -0, "0.000000" }
));