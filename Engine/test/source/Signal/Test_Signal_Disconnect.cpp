#include "gtest/gtest.h"
#include <iostream>
#include "Test_Base.hpp"

class Test_Signal_Disconnect : public SignalTest
{

};

TEST_F(Test_Signal_Disconnect, Test_Member_Disconnect)
{
    mo_signal_one.connect<&Foo::slot_member_signature_one>(mo_foo);
    mo_signal_one.connect<&Foo::slot_member_signature_one>(&mo_foo);
    EXPECT_FALSE_INFO(mo_signal_one.is_empty(), "A slot was found missing.");

    mo_signal_one.disconnect<&Foo::slot_member_signature_one>(mo_foo);
    mo_signal_one.disconnect<&Foo::slot_member_signature_one>(&mo_foo);
    EXPECT_TRUE_INFO(mo_signal_one.is_empty(), "A slot was found.");
    EXPECT_TRUE_INFO(mo_foo.is_empty(), "A slot was found.");

    //------------------------------------------------------------------

    mo_signal_two.connect<&Foo::slot_member_signature_two>(mo_foo);
    mo_signal_two.connect<&Foo::slot_member_signature_two>(&mo_foo);
    EXPECT_FALSE_INFO(mo_signal_two.is_empty(), "A slot was found missing.");

    mo_signal_two.disconnect<&Foo::slot_member_signature_two>(mo_foo);
    mo_signal_two.disconnect<&Foo::slot_member_signature_two>(&mo_foo);
    EXPECT_TRUE_INFO(mo_signal_two.is_empty(), "A slot was found.");
    EXPECT_TRUE_INFO(mo_foo.is_empty(), "A slot was found.");
}

TEST_F(Test_Signal_Disconnect, Test_Const_Member_Disconnect)
{
    mo_signal_one.connect<&Foo::slot_const_member_signature_one>(mo_foo);
    mo_signal_one.connect<&Foo::slot_const_member_signature_one>(&mo_foo);
    EXPECT_FALSE_INFO(mo_signal_one.is_empty(), "A slot was found missing.");

    mo_signal_one.disconnect<&Foo::slot_const_member_signature_one>(mo_foo);
    mo_signal_one.disconnect<&Foo::slot_const_member_signature_one>(&mo_foo);
    EXPECT_TRUE_INFO(mo_signal_one.is_empty(), "A slot was found.");
    EXPECT_TRUE_INFO(mo_foo.is_empty(), "A slot was found.");

    //------------------------------------------------------------------

    mo_signal_two.connect<&Foo::slot_const_member_signature_two>(mo_foo);
    mo_signal_two.connect<&Foo::slot_const_member_signature_two>(&mo_foo);
    EXPECT_FALSE_INFO(mo_signal_two.is_empty(), "A slot was found missing.");

    mo_signal_two.disconnect<&Foo::slot_const_member_signature_two>(mo_foo);
    mo_signal_two.disconnect<&Foo::slot_const_member_signature_two>(&mo_foo);
    EXPECT_TRUE_INFO(mo_signal_two.is_empty(), "A slot was found.");
    EXPECT_TRUE_INFO(mo_foo.is_empty(), "A slot was found.");
}

TEST_F(Test_Signal_Disconnect, Test_Overloaded_Member_Disconnect)
{
    mo_signal_one.connect<Foo, &Foo::slot_overloaded_member>(mo_foo);
    mo_signal_one.connect<Foo, &Foo::slot_overloaded_member>(&mo_foo);
    EXPECT_FALSE_INFO(mo_signal_one.is_empty(), "A slot was found missing.");

    mo_signal_one.disconnect<Foo, &Foo::slot_overloaded_member>(mo_foo);
    mo_signal_one.disconnect<Foo, &Foo::slot_overloaded_member>(&mo_foo);
    EXPECT_TRUE_INFO(mo_signal_one.is_empty(), "A slot was found.");
    EXPECT_TRUE_INFO(mo_foo.is_empty(), "A slot was found.");

    //------------------------------------------------------------------

    mo_signal_two.connect<Foo, &Foo::slot_overloaded_member>(mo_foo);
    mo_signal_two.connect<Foo, &Foo::slot_overloaded_member>(&mo_foo);
    EXPECT_FALSE_INFO(mo_signal_two.is_empty(), "A slot was found missing.");

    mo_signal_two.disconnect<Foo, &Foo::slot_overloaded_member>(mo_foo);
    mo_signal_two.disconnect<Foo, &Foo::slot_overloaded_member>(&mo_foo);
    EXPECT_TRUE_INFO(mo_signal_two.is_empty(), "A slot was found.");
    EXPECT_TRUE_INFO(mo_foo.is_empty(), "A slot was found.");
}

TEST_F(Test_Signal_Disconnect, Test_Overloaded_Static_Member_Disconnect)
{
    mo_signal_one.connect<&Foo::slot_static_member_function>();
    EXPECT_FALSE_INFO(mo_signal_one.is_empty(), "A slot was found missing.");

    mo_signal_two.connect<&Foo::slot_static_member_function>();
    EXPECT_FALSE_INFO(mo_signal_two.is_empty(), "A slot was found missing.");

    //------------------------------------------------------------------

    mo_signal_one.disconnect<&Foo::slot_static_member_function>();
    EXPECT_TRUE_INFO(mo_signal_one.is_empty(), "A slot was found.");

    mo_signal_two.disconnect<&Foo::slot_static_member_function>();
    EXPECT_TRUE_INFO(mo_signal_two.is_empty(), "A slot was found.");
}

TEST_F(Test_Signal_Disconnect, Test_Overloaded_Virtual_Member_Disconnect)
{
    mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
    mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(&mo_foo);
    EXPECT_FALSE_INFO(mo_signal_one.is_empty(), "A slot was found missing.");

    mo_signal_one.disconnect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
    mo_signal_one.disconnect<Foo, &Foo::slot_virtual_member_function>(&mo_foo);
    EXPECT_TRUE_INFO(mo_signal_one.is_empty(), "A slot was found.");
    EXPECT_TRUE_INFO(mo_foo.is_empty(), "A slot was found.");

    //------------------------------------------------------------------

    mo_signal_two.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
    mo_signal_two.connect<Foo, &Foo::slot_virtual_member_function>(&mo_foo);
    EXPECT_FALSE_INFO(mo_signal_two.is_empty(), "A slot was found missing.");

    mo_signal_two.disconnect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
    mo_signal_two.disconnect<Foo, &Foo::slot_virtual_member_function>(&mo_foo);
    EXPECT_TRUE_INFO(mo_signal_two.is_empty(), "A slot was found.");
    EXPECT_TRUE_INFO(mo_foo.is_empty(), "A slot was found.");
}

TEST_F(Test_Signal_Disconnect, Test_Overloaded_Virtual_Derived_Member_Disconnect)
{
    mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);
    mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(&mo_bar);
    EXPECT_FALSE_INFO(mo_signal_one.is_empty(), "A slot was found missing.");

    mo_signal_one.disconnect<Bar, &Bar::slot_virtual_member_function>(mo_bar);
    mo_signal_one.disconnect<Bar, &Bar::slot_virtual_member_function>(&mo_bar);
    EXPECT_TRUE_INFO(mo_signal_one.is_empty(), "A slot was found.");
    EXPECT_TRUE_INFO(mo_bar.is_empty(), "A slot was found.");

    //------------------------------------------------------------------

    mo_signal_two.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);
    mo_signal_two.connect<Bar, &Bar::slot_virtual_member_function>(&mo_bar);
    EXPECT_FALSE_INFO(mo_signal_two.is_empty(), "A slot was found missing.");

    mo_signal_two.disconnect<Bar, &Bar::slot_virtual_member_function>(mo_bar);
    mo_signal_two.disconnect<Bar, &Bar::slot_virtual_member_function>(&mo_bar);
    EXPECT_TRUE_INFO(mo_signal_two.is_empty(), "A slot was found.");
    EXPECT_TRUE_INFO(mo_bar.is_empty(), "A slot was found.");
}

TEST_F(Test_Signal_Disconnect, Test_Overloaded_Free_Function_Disconnect)
{
    mo_signal_one.connect<&slot_static_free_function>();
    EXPECT_FALSE_INFO(mo_signal_one.is_empty(), "A slot was found missing.");

    mo_signal_two.connect<&slot_static_free_function>();
    EXPECT_FALSE_INFO(mo_signal_two.is_empty(), "A slot was found missing.");

    //------------------------------------------------------------------

    mo_signal_one.disconnect<&slot_static_free_function>();
    EXPECT_TRUE_INFO(mo_signal_one.is_empty(), "A slot was found.");

    mo_signal_two.disconnect<&slot_static_free_function>();
    EXPECT_TRUE_INFO(mo_signal_two.is_empty(), "A slot was found.");
}

TEST_F(Test_Signal_Disconnect, Test_Disconnect_All)
{
    mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
    mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);
    mo_signal_one.connect<&slot_static_free_function>();
    EXPECT_FALSE_INFO(mo_signal_one.is_empty(), "A slot was found missing.");

    mo_signal_one.disconnect_all();
    EXPECT_TRUE_INFO(mo_signal_one.is_empty(), "A slot was found.");
    EXPECT_TRUE_INFO(mo_foo.is_empty(), "A slot was found.");
    EXPECT_TRUE_INFO(mo_bar.is_empty(), "A slot was found.");

    //------------------------------------------------------------------

    mo_signal_two.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
    mo_signal_two.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);
    mo_signal_two.connect<&slot_static_free_function>();
    EXPECT_FALSE_INFO(mo_signal_two.is_empty(), "A slot was found missing.");

    mo_signal_two.disconnect_all();
    EXPECT_TRUE_INFO(mo_signal_two.is_empty(), "A slot was found.");
    EXPECT_TRUE_INFO(mo_foo.is_empty(), "A slot was found.");
    EXPECT_TRUE_INFO(mo_bar.is_empty(), "A slot was found.");
}
