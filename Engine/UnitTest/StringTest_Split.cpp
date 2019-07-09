#include <gtest/gtest.h>
#include "Core/Common/String.h"

struct StringSplitParam 
{
    std::string str;
    std::string sep;
    std::vector<std::string> result;
    bool isRegex = false;
};

template<>
std::string testing::PrintToString(const StringSplitParam& param)
{
    return param.str;
}

class StringSplitTest : public testing::Test
                      , public testing::WithParamInterface<StringSplitParam>
{
public:
    virtual void SetUp() override 
    {}

    virtual void TearDown() override
    {}

protected:
    static bool StringVectorEqual(const std::vector<std::string>& real,
                                  const std::vector<std::string>& should)
    {
        EXPECT_EQ(real.size(), should.size());
        if (real.size() != should.size())
            return false;

        for (size_t i = 0; i < real.size(); i++) {
            EXPECT_EQ(real[i], should[i]);
        }

        return true;
    }
};

TEST_P(StringSplitTest, Split)
{
    const StringSplitParam& param = GetParam();

    std::vector<std::string> parts = ZHU::String::Split(param.str, param.sep, param.isRegex);
    StringVectorEqual(parts, param.result);
}

INSTANTIATE_TEST_CASE_P(Instantiation,
                        StringSplitTest,
                        ::testing::Values(
                            StringSplitParam{ "this*is*a*test", "*", {"this", "is", "a", "test"}, false },
                            StringSplitParam{ "this*is*a*test*", "*", {"this", "is", "a", "test"}, false },
                            StringSplitParam{ "*this*is*a**test**", "*", {"", "this", "is", "a", "", "test", ""}, false },
                            StringSplitParam{ "XXthisXXisXXaXXXXXtXestXX", "XX", {"", "this", "is", "a", "", "XtXest"}, false },
                            StringSplitParam{ "", "*", {""}, false },
                            StringSplitParam{ "***", "*", {"", "", ""}, false }
));