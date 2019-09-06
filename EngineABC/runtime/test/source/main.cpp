#include <gtest/gtest.h>

int main(int argc, char** argv)
{
    testing::GTEST_FLAG(output) = "xml:";
    testing::InitGoogleTest(&argc, argv);

    int err = RUN_ALL_TESTS();
    if (err != 0)
        system("pause");
    return 0;
}