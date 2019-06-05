#include <catch2/catch.hpp>
#include "CSignal.h"

using namespace GSZHU;

static int func1_return = 0;
static int func2_return = 0;
static int func3_return = 0;

int func1(float f) {
    func1_return = f * 2;
    return func1_return;
}

TEST_CASE("Test Signal/Slot in single thread", "[Signal/Slot]") {
    auto func2 = [&](float f)->int {  func2_return = f * 3; return func2_return; };

    CSignal_ST<int, float> S;
    CConection Conn = S.Connect(func1);
    REQUIRE(Conn.IsConnected());
    REQUIRE(S.SlotCount() == 1);

    CConection Conn2 = S.Connect(func2);
    REQUIRE(Conn2.IsConnected());
    REQUIRE(S.SlotCount() == 2);

    S(11.f);
    REQUIRE(func1_return == 22);
    REQUIRE(func2_return == 33);
    REQUIRE(Conn.IsConnected());
    REQUIRE(Conn2.IsConnected());
    REQUIRE(S.SlotCount() == 2);

    Conn.Disconnect();
    REQUIRE(S.SlotCount() == 1);
    REQUIRE_FALSE(Conn.IsConnected());
    REQUIRE(Conn2.IsConnected());

    func1_return = 0;
    S(33.f);
    REQUIRE(func1_return == 0);
    REQUIRE(func2_return == 99);

    // Pass reference as arguments
    auto func3 = [&](float f, const std::string& str, int& ref) {
        ref = 1122;
        func3_return = f * 4;
        return func3_return;
    };

    CSignal_ST<int, float, const std::string&, int&> SS;
    auto Conn3 = SS.Connect(func3);
    REQUIRE(Conn3.IsConnected());
    REQUIRE(SS.SlotCount() == 1);

    int value = 0;
    SS(10, "this", value);
    REQUIRE(func3_return == 40);
    REQUIRE(value == 1122);
}

