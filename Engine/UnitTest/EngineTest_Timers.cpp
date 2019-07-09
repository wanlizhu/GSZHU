#include <gtest/gtest.h>
#include <thread>
#include <ZHUENGINE/Core/Engine.h>
#include <ZHUENGINE/Core/Timers.h>

using namespace ZHU;

TEST(EngineTest, TimersModule)
{
    Engine::Initialize();

    int x = 0;
    auto timers = Timers::Get();
    timers->Repeat(Time::Milliseconds(10), 2, [&]() {
        x++;
    });

    std::this_thread::sleep_for((std::chrono::duration<int64_t, std::milli>)Time::Milliseconds(22));
    EXPECT_EQ(x, 2);
}