#include "gtest/gtest.h"
#include <iostream>
#include <functional>
#include "Test_Base.hpp"

class Test_Signal_Edge_Cases : public SignalTest
{

};

TEST_F(Test_Signal_Edge_Cases, Test_Fired_Disconnect)
{
    Delegate_One fo1;

    mo_signal_one.connect(fo1);
    mo_signal_one.connect<&Foo::slot_member_signature_one>(mo_foo);
    mo_signal_one.connect<&Foo::slot_const_member_signature_one>(mo_foo);
    mo_signal_one.connect<Foo, &Foo::slot_overloaded_member>(mo_foo);
    mo_signal_one.connect<&Foo::slot_static_member_function>();
    mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
    mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);

    fo1 = [&] (const char* sl) {
        mo_signal_one.disconnect(fo1);
    };

    mo_signal_one.fire(__FILE__);
    EXPECT_STREQ(anonymous_output::test, __FILE__);
    anonymous_output::reset();
}

TEST_F(Test_Signal_Edge_Cases, Test_Fire_Disconnects)
{
    Delegate_One fo1;

    mo_signal_one.connect(fo1);
    mo_signal_one.connect<&Foo::slot_member_signature_one>(mo_foo);
    mo_signal_one.connect<&Foo::slot_const_member_signature_one>(mo_foo);
    mo_signal_one.connect<Foo, &Foo::slot_overloaded_member>(mo_foo);
    mo_signal_one.connect<&Foo::slot_static_member_function>();
    mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
    mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);

    fo1 = [&] (const char* sl) {
        mo_signal_one.disconnect(fo1);
        mo_signal_one.disconnect<&Foo::slot_member_signature_one>(mo_foo);
        mo_signal_one.disconnect<&Foo::slot_const_member_signature_one>(mo_foo);
        mo_signal_one.disconnect<Foo, &Foo::slot_overloaded_member>(mo_foo);
        mo_signal_one.disconnect<&Foo::slot_static_member_function>();
        mo_signal_one.disconnect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
        mo_signal_one.disconnect<Bar, &Bar::slot_virtual_member_function>(mo_bar);
    };

    mo_signal_one.fire(__FILE__);
    EXPECT_STREQ(anonymous_output::test, __FILE__);
    anonymous_output::reset();

    EXPECT_TRUE_INFO(mo_signal_one.is_empty(), "A slot was found.");
}

TEST_F(Test_Signal_Edge_Cases, Test_Fire_Connects)
{
    Delegate_One fo1;

    mo_signal_one.connect(fo1);
    EXPECT_EQ(mo_signal_one.slots_count(), 1);

    fo1 = [&] (const char* sl) {
        mo_signal_one.connect(fo1);
        mo_signal_one.connect<&Foo::slot_member_signature_one>(mo_foo);
        mo_signal_one.connect<&Foo::slot_const_member_signature_one>(mo_foo);
        mo_signal_one.connect<Foo, &Foo::slot_overloaded_member>(mo_foo);
        mo_signal_one.connect<&Foo::slot_static_member_function>();
        mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
        mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);
    };

    mo_signal_one.fire(__FILE__);
    EXPECT_EQ(mo_signal_one.slots_count(), 8);
    anonymous_output::reset();
}

TEST_F(Test_Signal_Edge_Cases, Test_Fire_Disconnect_All)
{
    Delegate_One fo1;

    mo_signal_one.connect(fo1);
    mo_signal_one.connect<&Foo::slot_member_signature_one>(mo_foo);
    mo_signal_one.connect<&Foo::slot_const_member_signature_one>(mo_foo);
    mo_signal_one.connect<Foo, &Foo::slot_overloaded_member>(mo_foo);
    mo_signal_one.connect<&Foo::slot_static_member_function>();
    mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
    mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);

    fo1 = [&] (const char* sl) {
        mo_signal_one.disconnect_all();
    };

    mo_signal_one.fire(__FILE__);
    EXPECT_STREQ(anonymous_output::test, __FILE__);
    anonymous_output::reset();

    EXPECT_TRUE_INFO(mo_signal_one.is_empty(), "A slot was found.");
}

TEST_F(Test_Signal_Edge_Cases, Test_Fire_Recursive_Fire)
{
    Delegate_One fo1;

    mo_signal_one.connect(fo1);
    mo_signal_one.connect<&Foo::slot_member_signature_one>(mo_foo);
    mo_signal_one.connect<&Foo::slot_const_member_signature_one>(mo_foo);
    mo_signal_one.connect<Foo, &Foo::slot_overloaded_member>(mo_foo);
    mo_signal_one.connect<&Foo::slot_static_member_function>();
    mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
    mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);

    fo1 = [&] (const char* sl) {
        // TODO Prevent this infinite loop
        // This would require a custom data structure for Observer
        // in order to realize a no-cost solution to this problem

        //mo_signal_one.fire(__FILE__);
    };

    mo_signal_one.fire(__FILE__);
    EXPECT_STREQ(anonymous_output::test, __FILE__);
    anonymous_output::reset();
}

