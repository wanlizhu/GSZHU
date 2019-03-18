#include <gtest/gtest.h>
#include "Core/ThreadPool.h"

using namespace NTGS;

void FreeFunc(int& V) {
    V = 999;
}

TEST(Core_ThreadPool, Post_TaskWithReference) {
    ThreadPool* Pool = ThreadPool::GetInstance();
    std::string Str;
    int X = 0;
    auto Result = Pool->Enqueue([](int Value) { return Value * 2; }, 12);
    auto Result3 = Pool->Enqueue(FreeFunc, std::ref(X));


    EXPECT_EQ(Result.get(), 24);
    EXPECT_EQ(Pool->CountTask(), 0);
    EXPECT_EQ(Pool->CountWorker(), std::thread::hardware_concurrency());

    Result3.get();
    EXPECT_EQ(X, 999);
}