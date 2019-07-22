#pragma once

#include <mutex>
#include <functional>
#include "gtest/gtest.h"
#include "Signal/Signal.h"
#include "Signal/Mutex.h"

#define ZHU_THREAD_SAFE_SIGNALS
// the value of a comma expression is equivalent to its last(rightmost) operand 
#define EXPECT_TRUE_INFO(condition, info)  EXPECT_TRUE((info, condition))
#define EXPECT_FALSE_INFO(condition, info) EXPECT_FALSE((info, condition))

namespace
{
    static void anonymous_output(const char* fn, const char* sl, std::size_t ln)
    {
        std::cout << fn << " LINE: " << __LINE__ << " Test: " << sl << " LINE: " << ln << std::endl;
    }
}

#if defined(ZHU_THREAD_SAFE_SIGNALS)
using Observer = ZHU::Observer<ZHU::RecursiveMutex>;
using Signal_One = ZHU::Signal<void(const char*), ZHU::RecursiveMutex>;
using Signal_Two = ZHU::Signal<void(const char*, std::size_t), ZHU::RecursiveMutex>;
#else
using Observer = ZHU::Observer<>;
using Signal_One = ZHU::Signal<void(const char*)>;
using Signal_Two = ZHU::Signal<void(const char*, std::size_t)>;
#endif

using Delegate_One = std::function<void(const char*)>;
using Delegate_Two = std::function<void(const char*, std::size_t)>;

//--------------------------------------------------------------------------

class Foo : public Observer
{
public:

    void slot_member_signature_one(const char* sl)
    {
        anonymous_output(__FUNCTION__, sl, __LINE__);
    }
    void slot_member_signature_two(const char* sl, std::size_t ln)
    {
        anonymous_output(__FUNCTION__, sl, ln);
    }

    void slot_const_member_signature_one(const char* sl) const
    {
        anonymous_output(__FUNCTION__, sl, __LINE__);
    }
    void slot_const_member_signature_two(const char* sl, std::size_t ln) const
    {
        anonymous_output(__FUNCTION__, sl, ln);
    }

    void slot_overloaded_member(const char* sl)
    {
        anonymous_output(__FUNCTION__, sl, __LINE__);
    }
    void slot_overloaded_member(const char* sl, std::size_t ln)
    {
        anonymous_output(__FUNCTION__, sl, ln);
    }

    static void slot_static_member_function(const char* sl)
    {
        anonymous_output(__FUNCTION__, sl, __LINE__);
    }
    static void slot_static_member_function(const char* sl, std::size_t ln)
    {
        anonymous_output(__FUNCTION__, sl, ln);
    }

    virtual void slot_virtual_member_function(const char* sl)
    {
        anonymous_output(__FUNCTION__, sl, __LINE__);
    }
    virtual void slot_virtual_member_function(const char* sl, std::size_t ln)
    {
        anonymous_output(__FUNCTION__, sl, ln);
    }
};

//--------------------------------------------------------------------------

class Bar : public Foo
{
public:

    void slot_virtual_member_function(const char* sl) override
    {
        anonymous_output(__FUNCTION__, sl, __LINE__);
    }
    void slot_virtual_member_function(const char* sl, std::size_t ln) override
    {
        anonymous_output(__FUNCTION__, sl, ln);
    }
};

//--------------------------------------------------------------------------

class SignalTest : public testing::Test
{
protected:
    virtual void SetUp() override {}
    virtual void TearDown() override {}

protected:
    Signal_One mo_signal_one;
    Signal_Two mo_signal_two;

    Foo mo_foo;
    Bar mo_bar;
};

//--------------------------------------------------------------------------

static void slot_static_free_function(const char* sl)
{
    anonymous_output(__FUNCTION__, sl, __LINE__);
}

static void slot_static_free_function(const char* sl, std::size_t ln)
{
    anonymous_output(__FUNCTION__, sl, ln);
}

