#include "gtest/gtest.h"
#include <iostream>
#include <memory>
#include "Test_Base.hpp"

class Test_Observer_Destruction : public SignalTest
{

};

TEST_F(Test_Observer_Destruction, Test_Stack_Foo_Observer_Destructor)
{
    {
        Foo to_foo;
        mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(to_foo);
        mo_signal_two.connect<Foo, &Foo::slot_virtual_member_function>(&to_foo);
    }

    EXPECT_TRUE_INFO(mo_signal_one.is_empty(), "A slot was found.");
    EXPECT_TRUE_INFO(mo_signal_two.is_empty(), "A slot was found.");
}

TEST_F(Test_Observer_Destruction, Test_Stack_Bar_Foo_Observer_Destructor)
{
    {
        Bar to_bar;
        mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(&to_bar);
        mo_signal_two.connect<Bar, &Bar::slot_virtual_member_function>(to_bar);
    }

    EXPECT_TRUE_INFO(mo_signal_one.is_empty(), "A slot was found.");
    EXPECT_TRUE_INFO(mo_signal_two.is_empty(), "A slot was found.");
}

TEST_F(Test_Observer_Destruction, Test_Heap_Foo_Observer_Destructor)
{
    {
        auto to_foo = std::make_unique<Foo>();
        mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(*to_foo);
        mo_signal_two.connect<Foo, &Foo::slot_virtual_member_function>(*to_foo);
    }

    EXPECT_TRUE_INFO(mo_signal_one.is_empty(), "A slot was found.");
    EXPECT_TRUE_INFO(mo_signal_two.is_empty(), "A slot was found.");
}

TEST_F(Test_Observer_Destruction, Test_Heap_Bar_Foo_Observer_Destructor)
{
    {
        auto to_bar = std::make_unique<Bar>();
        mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(*to_bar);
        mo_signal_two.connect<Bar, &Bar::slot_virtual_member_function>(*to_bar);
    }

    EXPECT_TRUE_INFO(mo_signal_one.is_empty(), "A slot was found.");
    EXPECT_TRUE_INFO(mo_signal_two.is_empty(), "A slot was found.");
}
