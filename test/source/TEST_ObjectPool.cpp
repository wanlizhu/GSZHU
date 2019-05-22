#include <catch2/catch.hpp>
#include "GSZHU/INC/CObjectPool.h"
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <type_traits>

using namespace GSZHU;

// Default constructible dummy object
struct dummy_one {
    dummy_one() {
        ++m_count;
    }

    ~dummy_one() {
        --m_count;
    }

    // Counter which will check how many object have been allocate and deallocated
    static int32_t m_count;
};
int32_t dummy_one::m_count = 0;

std::shared_ptr<dummy_one> make_dummy_one() {
    return std::make_shared<dummy_one>();
}

// Non Default constructible dummy object
struct dummy_two {
    dummy_two(uint32_t) {
        ++m_count;
    }

    ~dummy_two() {
        --m_count;
    }

    static int32_t m_count;
};
int32_t dummy_two::m_count = 0;

std::shared_ptr<dummy_two> make_dummy_two(uint32_t v) {
    return std::make_shared<dummy_two>(v);
}

// enable_shared_from_this dummy object
struct dummy_three : std::enable_shared_from_this<dummy_three> {
    dummy_three() {
        ++m_count;
    }

    ~dummy_three() {
        --m_count;
    }

    // Counter which will check how many object have been allocate and deallocated
    static int32_t m_count;
};
int32_t dummy_three::m_count = 0;


template <class T>
struct is_regular
    : std::integral_constant<bool, std::is_default_constructible<T>::value &&
    std::is_copy_constructible<T>::value &&
    std::is_move_constructible<T>::value &&
    std::is_copy_assignable<T>::value &&
    std::is_move_assignable<T>::value>
{};


TEST_CASE("Is regular type", "[Object Pool]")
{
    REQUIRE(is_regular<CObjectPool<dummy_one>>::value);
    REQUIRE_FALSE(is_regular<CObjectPool<dummy_two>>::value);
}

TEST_CASE("Basic API construct and free some objects", "[Object Pool]")
{
    {
        CObjectPool<dummy_one> pool;

        REQUIRE(pool.GetFreeObjectCount() == 0U);

        {
            auto d1 = pool.Allocate();
            REQUIRE(pool.GetFreeObjectCount() == 0U);
        }

        REQUIRE(pool.GetFreeObjectCount() == 1U);

        auto d2 = pool.Allocate();

        REQUIRE(pool.GetFreeObjectCount() == 0U);

        auto d3 = pool.Allocate();

        REQUIRE(pool.GetFreeObjectCount() == 0U);
        REQUIRE(dummy_one::m_count == 2);

        {
            auto d4 = pool.Allocate();
            REQUIRE(pool.GetFreeObjectCount() == 0U);
        }

        REQUIRE(pool.GetFreeObjectCount() == 1U);

        pool.Shrink();

        REQUIRE(pool.GetFreeObjectCount() == 0U);
    }

    REQUIRE(dummy_one::m_count == 0);
}

TEST_CASE("Works with std::bind", "[Object Pool]")
{
    {
        CObjectPool<dummy_one> pool_one(std::bind(make_dummy_one));

        CObjectPool<dummy_two> pool_two(std::bind(make_dummy_two, 4U));

        auto o1 = pool_one.Allocate();
        auto o2 = pool_two.Allocate();

        REQUIRE(dummy_one::m_count == 1);
        REQUIRE(dummy_two::m_count == 1);
    }

    REQUIRE(dummy_one::m_count == 0);
    REQUIRE(dummy_two::m_count == 0);
}

TEST_CASE("Works for non default constructable objects", "[Object Pool]")
{
    {
        CObjectPool<dummy_two> pool(std::bind(make_dummy_two, 4U));

        auto o1 = pool.Allocate();
        auto o2 = pool.Allocate();

        REQUIRE(dummy_two::m_count == 2);
    }

    REQUIRE(dummy_two::m_count == 0);

    {
        // clang-format off
        auto make = []() -> std::shared_ptr<dummy_two> {
            return std::make_shared<dummy_two>(3U);
        };
        // clang-format on

        CObjectPool<dummy_two> pool(make);

        auto o1 = pool.Allocate();
        auto o2 = pool.Allocate();

        REQUIRE(dummy_two::m_count == 2);
    }

    REQUIRE(dummy_two::m_count == 0);
}

TEST_CASE("Works for non constructable objects", "[Object Pool]")
{
    {
        CObjectPool<dummy_one> pool;

        auto o1 = pool.Allocate();
        auto o2 = pool.Allocate();

        REQUIRE(dummy_one::m_count == 2);
    }

    REQUIRE(dummy_one::m_count == 0);
}

TEST_CASE("The pool dies before the objects allocated", "[Object Pool]")
{
    {
        std::shared_ptr<dummy_one> d1;
        std::shared_ptr<dummy_one> d2;
        std::shared_ptr<dummy_one> d3;

        {
            CObjectPool<dummy_one> pool;

            d1 = pool.Allocate();
            d2 = pool.Allocate();
            d3 = pool.Allocate();

            REQUIRE(dummy_one::m_count == 3);
        }

        REQUIRE(dummy_one::m_count == 3);
    }

    REQUIRE(dummy_one::m_count == 0);
}


TEST_CASE("Test that the recycle functionality works", "[Object Pool]")
{
    uint32_t recycled = 0;

    // clang-format off
    auto recycle = [&recycled](std::shared_ptr<dummy_two> o) {
        REQUIRE((bool)o);
        ++recycled;
    };

    auto make = []() -> std::shared_ptr<dummy_two> {
        return std::make_shared<dummy_two>(3U);
    };
    // clang-format on

    CObjectPool<dummy_two> pool(make, recycle);

    auto o1 = pool.Allocate();
    o1.reset();

    REQUIRE(recycled == 1U);
}

TEST_CASE("Test copy constructor", "[Object Pool]")
{
    CObjectPool<dummy_one> pool;

    auto o1 = pool.Allocate();
    auto o2 = pool.Allocate();

    o1.reset();

    CObjectPool<dummy_one> new_pool(pool);

    REQUIRE(pool.GetFreeObjectCount() == 1U);
    REQUIRE(new_pool.GetFreeObjectCount() == 1U);

    o2.reset();

    REQUIRE(pool.GetFreeObjectCount() == 2U);
    REQUIRE(new_pool.GetFreeObjectCount() == 1U);

    REQUIRE(dummy_one::m_count == 3);

    pool.Shrink();
    new_pool.Shrink();

    REQUIRE(dummy_one::m_count == 0);
}

TEST_CASE("Test copy assignment", "[Object Pool]")
{
    CObjectPool<dummy_one> pool;

    auto o1 = pool.Allocate();
    auto o2 = pool.Allocate();

    o1.reset();

    CObjectPool<dummy_one> new_pool;
    new_pool = pool;

    REQUIRE(dummy_one::m_count == 3);
    auto o3 = new_pool.Allocate();
    REQUIRE(dummy_one::m_count == 3);
}

TEST_CASE("Test move constructor", "[Object Pool]")
{
    CObjectPool<dummy_one> pool;

    auto o1 = pool.Allocate();
    auto o2 = pool.Allocate();

    o1.reset();

    CObjectPool<dummy_one> new_pool(std::move(pool));

    o2.reset();
    REQUIRE(new_pool.GetFreeObjectCount() == 2U);
}

TEST_CASE("Test move assignment", "[Object Pool]")
{
    CObjectPool<dummy_one> pool;

    auto o1 = pool.Allocate();
    auto o2 = pool.Allocate();

    o1.reset();

    CObjectPool<dummy_one> new_pool;
    new_pool = std::move(pool);

    o2.reset();

    REQUIRE(new_pool.GetFreeObjectCount() == 2U);
}

TEST_CASE("Test copy recycle", "[Object Pool]")
{
    uint32_t recycled = 0;

    // clang-format off
    auto recycle = [&recycled](std::shared_ptr<dummy_two> o) {
        REQUIRE((bool)o);
        ++recycled;
    };

    auto make = []() -> std::shared_ptr<dummy_two> {
        return std::make_shared<dummy_two>(3U);
    };
    // clang-format on

    CObjectPool<dummy_two> pool(make, recycle);
    CObjectPool<dummy_two> new_pool = pool;

    REQUIRE(pool.GetFreeObjectCount() == 0U);
    REQUIRE(new_pool.GetFreeObjectCount() == 0U);

    auto o1 = new_pool.Allocate();

    REQUIRE(dummy_two::m_count == 1);

    o1.reset();
    REQUIRE(recycled == 1U);

    new_pool.Shrink();

    REQUIRE(dummy_two::m_count == 0);
}

struct lock_policy
{
    using Mutex = std::mutex;
    using Lock = std::lock_guard<Mutex>;
};


TEST_CASE("Test multi-thread", "[Object Pool]")
{
    uint32_t recycled = 0;

    // clang-format off
    auto recycle = [&recycled](std::shared_ptr<dummy_two> o) {
        REQUIRE((bool)o);
        ++recycled;
    };

    auto make = []() -> std::shared_ptr<dummy_two> {
        return std::make_shared<dummy_two>(3U);
    };
    // clang-format on

    // The pool we will use
    using pool_type = CObjectPool<dummy_two, lock_policy>;

    pool_type pool(make, recycle);

    // Lambda the threads will execute captures a reference to the pool
    // so they will all operate on the same pool concurrently
    // clang-format off
    auto run = [&pool]() {
        {
            auto a1 = pool.Allocate();
        }

        auto a2 = pool.Allocate();
        auto a3 = pool.Allocate();

        {
            auto a4 = pool.Allocate();
        }

        pool_type new_pool = pool;

        auto b1 = new_pool.Allocate();
        auto b2 = new_pool.Allocate();

        pool.Shrink();
    };
    // clang-format on

    const uint32_t number_threads = 8;
    std::thread t[number_threads];

    // Launch a group of threads
    for (uint32_t i = 0; i < number_threads; ++i) {
        t[i] = std::thread(run);
    }

    // Join the threads with the main thread
    for (uint32_t i = 0; i < number_threads; ++i) {
        t[i].join();
    }
}

TEST_CASE("Test std::enable_shared_from_this<...>", "[Object Pool]")
{
    {
        CObjectPool<dummy_three> pool;

        auto o1 = pool.Allocate();
        REQUIRE(o1.use_count() == 1);

        REQUIRE(dummy_three::m_count == 1);
    }

    REQUIRE(dummy_three::m_count == 0);
}
