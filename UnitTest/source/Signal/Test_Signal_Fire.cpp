#include "gtest/gtest.h"
#include <iostream>
#include <vector>
#include "Test_Base.hpp"

class Test_Signal_Fire : public SignalTest
{

};

TEST_F(Test_Signal_Fire, Test_Member_Fire)
{
    mo_signal_one.connect<&Foo::slot_member_signature_one>(mo_foo);
    mo_signal_two.connect<&Foo::slot_member_signature_two>(mo_foo);

    mo_signal_one.fire(__FUNCTION__);
    EXPECT_STREQ(anonymous_output::test, __FUNCTION__);
    anonymous_output::reset();

    std::size_t line = __LINE__;
    mo_signal_two.fire(__FUNCTION__, std::move(line));
    EXPECT_STREQ(anonymous_output::test, __FUNCTION__);
    EXPECT_EQ(anonymous_output::line, line);
    anonymous_output::reset();
}

TEST_F(Test_Signal_Fire, Test_Const_Member_Fire)
{
    mo_signal_one.connect<&Foo::slot_const_member_signature_one>(mo_foo);
    mo_signal_two.connect<&Foo::slot_const_member_signature_two>(mo_foo);

    mo_signal_one.fire(__FUNCTION__);
    EXPECT_STREQ(anonymous_output::test, __FUNCTION__);
    anonymous_output::reset();

    std::size_t line = __LINE__;
    mo_signal_two.fire(__FUNCTION__, line);
    EXPECT_STREQ(anonymous_output::test, __FUNCTION__);
    EXPECT_EQ(anonymous_output::line, line);
    anonymous_output::reset();
}

TEST_F(Test_Signal_Fire, Test_Overloaded_Member_Fire)
{
    mo_signal_one.connect<Foo, &Foo::slot_overloaded_member>(mo_foo);
    mo_signal_two.connect<Foo, &Foo::slot_overloaded_member>(mo_foo);

    mo_signal_one.fire(__FUNCTION__);
    EXPECT_STREQ(anonymous_output::test, __FUNCTION__);
    anonymous_output::reset();

    std::size_t line = __LINE__;
    mo_signal_two.fire(__FUNCTION__, line);
    EXPECT_STREQ(anonymous_output::test, __FUNCTION__);
    EXPECT_EQ(anonymous_output::line, line);
    anonymous_output::reset();
}

TEST_F(Test_Signal_Fire, Test_Overloaded_Static_Member_Fire)
{
    mo_signal_one.connect<&Foo::slot_static_member_function>();
    mo_signal_two.connect<&Foo::slot_static_member_function>();

    mo_signal_one.fire(__FUNCTION__);
    EXPECT_STREQ(anonymous_output::test, __FUNCTION__);
    anonymous_output::reset();

    std::size_t line = __LINE__;
    mo_signal_two.fire(__FUNCTION__, line);
    EXPECT_STREQ(anonymous_output::test, __FUNCTION__);
    EXPECT_EQ(anonymous_output::line, line);
    anonymous_output::reset();
}

TEST_F(Test_Signal_Fire, Test_Overloaded_Virtual_Member_Fire)
{
    mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
    mo_signal_two.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);

    mo_signal_one.fire(__FUNCTION__);
    EXPECT_STREQ(anonymous_output::test, __FUNCTION__);
    anonymous_output::reset();

    std::size_t line = __LINE__;
    mo_signal_two.fire(__FUNCTION__, line);
    EXPECT_STREQ(anonymous_output::test, __FUNCTION__);
    EXPECT_EQ(anonymous_output::line, line);
    anonymous_output::reset();
}

TEST_F(Test_Signal_Fire, Test_Overloaded_Virtual_Derived_Member_Fire)
{
    mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);
    mo_signal_two.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);

    mo_signal_one.fire(__FUNCTION__);
    EXPECT_STREQ(anonymous_output::test, __FUNCTION__);
    anonymous_output::reset();

    std::size_t line = __LINE__;
    mo_signal_two.fire(__FUNCTION__, line);
    EXPECT_STREQ(anonymous_output::test, __FUNCTION__);
    EXPECT_EQ(anonymous_output::line, line);
    anonymous_output::reset();
}

TEST_F(Test_Signal_Fire, Test_Fire_Accumulate)
{
    ZHU::Signal<std::size_t(std::size_t)> signal_three;
    auto slot_three = [&] (std::size_t val) {
        return val * val;
    };
    signal_three.connect(slot_three);

    std::vector<std::size_t> signal_return_values;
    auto accumulator = [&] (std::size_t srv) {
        signal_return_values.push_back(srv);
    };

    signal_three.fire_accumulate(accumulator, __LINE__);
    signal_three.fire_accumulate(accumulator, __LINE__);
    signal_three.fire_accumulate(accumulator, __LINE__);
    signal_three.fire_accumulate(accumulator, __LINE__);
    signal_three.fire_accumulate(accumulator, __LINE__);
    signal_three.fire_accumulate(accumulator, __LINE__);
    signal_three.fire_accumulate(accumulator, __LINE__);
    signal_three.fire_accumulate(accumulator, __LINE__);
    signal_three.fire_accumulate(accumulator, __LINE__);
    signal_three.fire_accumulate(accumulator, __LINE__);

    EXPECT_TRUE_INFO(signal_return_values.size() == 10, "An SRV was found missing.");
}
