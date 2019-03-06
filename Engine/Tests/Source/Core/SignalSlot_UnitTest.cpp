#include <gtest/gtest.h>
#include <NTGS/Core/SignalSlot.h>
#include <memory>
#include <sstream>

using namespace NTGS;

namespace {
    // free function to output a string to a ostream
    void Output(std::ostream& Out, std::string const& Str) {
        Out << Str;
    }

    // free function that prints "one" to an ostream
    void OutputOne(std::ostream& Out) {
        Output(Out, "one");
    }

    // std::function<> that outputs "two" to an ostream
    std::function<void(std::ostream&)> OutputTwo =
        [](std::ostream& Out) {
        Output(Out, "two");
    };

    //test class
    struct STest {
        STest(std::string const& str) :
            _Str(str)
        {}

        void operator()(std::ostream& Out) const {
            Out << _Str;
        }

        std::string _Str;
    };
}

TEST(Core_SignalSlot, Connection_Test) {
    {
        Connection Conn;
        EXPECT_FALSE(Conn.IsConnected());
    }

    {
        Signal<void()> Sig;
        auto Conn = Sig.Connect([](){});
        EXPECT_TRUE(Conn.IsConnected());
        
        Conn.Disconnect();
        EXPECT_FALSE(Conn.IsConnected());
    }
    
    {
        std::ostringstream Stream;
        Signal<void(double)> Sig;
        auto Conn = Sig.Connect([&Stream](double X) { 
            Stream << "Signaled!"; 
        });
        
        Sig(12.5);
        EXPECT_TRUE(Stream.str() == "Signaled!");
        
        Conn.Disconnect();
        Stream = std::ostringstream();
        Sig(12.5);
        EXPECT_TRUE(Stream.str().empty());
    }
    
    {
        Signal<void(std::ostream&)> Sig;
        auto Conn1 = Sig.Connect([](std::ostream& O){ O << "One"; });
        auto Conn2 = Sig.Connect([](std::ostream& O){ O << "Two"; });
        auto Conn3 = Sig.Connect([](std::ostream& O){ O << "Three"; });
        
        std::ostringstream Stream;
        Sig(Stream);
        EXPECT_TRUE(Stream.str() == "OneTwoThree");
        
        Stream = std::ostringstream();
        Conn2.Disconnect();
        Sig(Stream);
        EXPECT_TRUE(Stream.str() == "OneThree");

        Stream = std::ostringstream();
        auto Conn4 = Sig.Connect([](std::ostream& O){ O << "Four"; });
        Sig(Stream);
        EXPECT_TRUE(Stream.str() == "OneThreeFour");
    }

    {
        auto SigPtr = std::make_unique<Signal<void()>>();
        auto Conn1 = SigPtr->Connect([](){});
        auto Conn2 = SigPtr->Connect([](){});
        auto Conn3 = SigPtr->Connect([](){});

        EXPECT_TRUE(Conn1.IsConnected());
        EXPECT_TRUE(Conn2.IsConnected());
        EXPECT_TRUE(Conn3.IsConnected());

        SigPtr.reset();
        EXPECT_FALSE(Conn1.IsConnected());
        EXPECT_FALSE(Conn2.IsConnected());
        EXPECT_FALSE(Conn3.IsConnected());
    }
}

TEST(Core_SignalSlot, ScopedConnection_Test) {
    {
        EXPECT_TRUE(std::is_default_constructible<ScopedConnection>::value);
        ScopedConnection ScopedConn;
        EXPECT_FALSE(ScopedConn.IsConnected());
    }

    {
        EXPECT_TRUE(std::is_move_constructible<ScopedConnection>::value);
        Signal<void()> Sig;
        ScopedConnection ScopedConn = Sig.Connect([](){});

        ScopedConnection Conn2{ std::move(ScopedConn) };
        EXPECT_TRUE(Conn2.IsConnected());
        EXPECT_FALSE(ScopedConn.IsConnected());
        EXPECT_TRUE(Sig.SlotCount() == 1);
    }

    {
        EXPECT_FALSE(std::is_copy_constructible<ScopedConnection>::value);
        EXPECT_FALSE(std::is_copy_assignable<ScopedConnection>::value);
    }

    {
        EXPECT_TRUE(std::is_move_assignable<ScopedConnection>::value);
        Signal<void()> Sig;
        std::ostringstream Stream;
        ScopedConnection Scoped1 = Sig.Connect([&](){ Stream << "Signaled!"; });

        ScopedConnection Scoped2 = std::move(Scoped1);
        EXPECT_TRUE(Scoped2.IsConnected());
        EXPECT_FALSE(Scoped1.IsConnected());

        Stream = std::ostringstream();
        Sig();
        EXPECT_TRUE(Stream.str() == "Signaled!");
    }

    {
        Signal<void()> Sig;
        std::ostringstream Stream;
        ScopedConnection Conn1 = Sig.Connect([&](){ Stream << "1"; });
        ScopedConnection Conn2 = Sig.Connect([&](){ Stream << "2"; });
        EXPECT_TRUE(Sig.SlotCount() == 2);

        Conn1 = std::move(Conn2);
        EXPECT_TRUE(Sig.SlotCount() == 1);

        Sig();
        EXPECT_TRUE(Stream.str() == "2");
    }

    {
        std::ostringstream Stream;
        Signal<void()> Sig;
        auto Conn = std::make_unique<ScopedConnection>(Sig.Connect([&](){ Stream << "Signaled!"; }));

        EXPECT_TRUE(Conn->IsConnected());
        Conn.reset();
        Sig();
        EXPECT_TRUE(Stream.str().empty());
    }

    {
        std::ostringstream Stream;
        Signal<void()> Sig;
        ScopedConnection Scoped = Sig.Connect([&](){ Stream << "Signaled!"; });

        EXPECT_TRUE(Scoped.IsConnected());
        Scoped.Reset();
        EXPECT_FALSE(Scoped.IsConnected());
        Sig();
        EXPECT_TRUE(Stream.str().empty());
        
        Stream = std::ostringstream();
        Scoped.Reset(Sig.Connect([&](){ Stream << "Honk!"; }));
        EXPECT_TRUE(Scoped.IsConnected());
        Sig();
        EXPECT_TRUE(Stream.str() == "Honk!");
    }

    {
        std::ostringstream Stream;
        Signal<void()> Sig;
        ScopedConnection Scoped = Sig.Connect([&](){ Stream << "Signaled!"; });
        EXPECT_TRUE(Scoped.IsConnected());

        Connection Conn = Scoped.Detach();
        EXPECT_FALSE(Scoped.IsConnected());
        EXPECT_TRUE(Conn.IsConnected());
        Sig();
        EXPECT_TRUE(Stream.str() == "Signaled!");
    }
}

TEST(Core_SignalSlot, Disconnect_Test) {
    std::ostringstream Stream;
    Signal<void(const std::string&)> Sig;
    auto Conn1 = Sig.Connect([&Stream] (const std::string& Str) {
        Stream << "1: " << Str << "\n";
    });
    auto Conn2 = Sig.Connect([&Stream](const std::string& Str) {
        Stream << "2: " << Str << "\n";
    });

    EXPECT_TRUE(Conn1.IsConnected());
    EXPECT_TRUE(Conn2.IsConnected());

    Sig("testing");
    EXPECT_TRUE(Stream.str() == "1: testing\n2: testing\n");

    Sig.DisconnectAll();
    EXPECT_FALSE(Conn1.IsConnected());
    EXPECT_FALSE(Conn2.IsConnected());

    Stream = std::ostringstream();
    Sig("again");
    EXPECT_TRUE(Stream.str() == "");

    Stream = std::ostringstream();
    auto Conn3 = Sig.Connect([&Stream](const std::string& Str) {
        Stream << "3: " << Str << "\n";
    });
    EXPECT_TRUE(Conn3.IsConnected());
    Sig("third time");
    EXPECT_TRUE(Stream.str() == "3: third time\n");
}

TEST(Core_SignalSlot, General_Test) {
    using OutputSignalType = Signal<void(std::ostream&)>;

    {
        OutputSignalType S;
        S.Connect([](std::ostream& O) {
            O << "lambda";
        });
        std::stringstream SS;
        S(SS);
        EXPECT_TRUE(SS.str() == "lambda");
    }

    {
        OutputSignalType S;
        S.Connect(OutputOne);
        std::stringstream SS;
        S(SS);
        EXPECT_TRUE(SS.str() == "one");
    }

    {
        OutputSignalType S;
        S.Connect(OutputTwo);
        std::stringstream SS;
        S(SS);
        EXPECT_TRUE(SS.str() == "two");
    }

    {
        OutputSignalType S;
        S.Connect(std::bind(Output, std::placeholders::_1, "three"));
        std::stringstream SS;
        S(SS);
        EXPECT_TRUE(SS.str() == "three");
    }

    {
        OutputSignalType S;
        STest Functor{ "four" };
        S.Connect(Functor);
        std::stringstream SS;
        S(SS);
        EXPECT_TRUE(SS.str() == "four");
    }

    {
        std::stringstream SS;
        Signal<void(int)> S;

        S.Connect([&SS](int Value) {
            SS << "A: " << Value << ",";
        });
        S.Connect([&SS](int Value) {
            SS << "B: " << Value << ",";
        });

        S(12);
        S(42);

        EXPECT_TRUE(SS.str() == "A: 12,B: 12,A: 42,B: 42,");
    }

    {
        auto X = 0;
        Signal<void(int)> S;

        Connection Conn = S.Connect([&] (int Value) {
            X += Value;
            Conn.Disconnect();
        });

        S(5);
        EXPECT_TRUE(X == 5);
        S(10);
        EXPECT_TRUE(X == 5);
    }
}

TEST(Core_SignalSlot, Move_Constructed) {
    Signal<void(std::ostream&)> S;
    auto Conn = S.Connect(
        [](std::ostream& out) {
        out << "lambda";
    });
    Signal<void(std::ostream&)> MoveConstructedSignal{ std::move(S) };

    {
        std::stringstream SS;
        MoveConstructedSignal(SS);
        EXPECT_TRUE(SS.str() == "lambda");
    }

    {
        std::stringstream SS;
        Conn.Disconnect();
        MoveConstructedSignal(SS);
        EXPECT_TRUE(SS.str() == "");
    }
}

TEST(Core_SignalSlot, Move_Assigned) {
    Signal<void(std::ostream&)> signal;
    auto Conn = signal.Connect(
        [](std::ostream& out) {
        out << "lambda";
    });

    Signal<void(std::ostream&)> MoveAssignedSignal;
    auto Conn2 = MoveAssignedSignal.Connect(
        [](std::ostream& out) {
        out << "lambda2";
    });

    MoveAssignedSignal = std::move(signal);

    EXPECT_FALSE(Conn2.IsConnected());

    {
        std::stringstream ss;
        MoveAssignedSignal(ss);
        EXPECT_TRUE(ss.str() == "lambda");
    }

    {
        std::stringstream ss;
        Conn.Disconnect();
        MoveAssignedSignal(ss);
        EXPECT_TRUE(ss.str() == "");
    }
}

TEST(Core_SignalSlot, Move_Assigning_With_State) {
    Signal<void(std::ostream&)> Signal1;
    auto Conn1 = Signal1.Connect([](std::ostream& out) {
        out << "1";
    });
    Signal<void(std::ostream&)> Signal2;
    auto Conn2 = Signal2.Connect([](std::ostream& out) {
        out << "2";
    });

    Signal1 = std::move(Signal2);

    // The moved-to instance has disconnected it's original connection
    EXPECT_FALSE(Conn1.IsConnected());
    // The moved-from instance has moved not disconnected its original connection,
    // it is now owned by the moved-to instance
    EXPECT_TRUE(Conn2.IsConnected());

    // Triggering the moved-to signal has expected output
    std::stringstream SS;
    Signal1(SS);
    EXPECT_TRUE(SS.str() == "2");
}

TEST(Core_SignalSlot, Accumulate_Return_Values_Of_Slots) {
    Signal<double(double, double)> S;
    S.Connect(std::multiplies<double>{});
    S.Connect(std::plus<double>{});
    S.Connect(std::minus<double>{});
    
    {
        auto TheAccumulator = S.Accumulate(0.0, std::plus<double>{});
        auto Result = TheAccumulator(42, 12);
        EXPECT_TRUE(Result == (std::multiplies<double>{}(42, 12) + std::plus<double>{}(42, 12) + std::minus<double>{}(42, 12)));
    }

    {
        auto Result = S.Accumulate(std::vector<double>{}, [](std::vector<double> Partial, double SlotResult) {
            Partial.push_back(SlotResult);
            return Partial;
        })(42.0, 12.0);
        EXPECT_TRUE(Result == (std::vector<double>{std::multiplies<double>{}(42, 12), std::plus<double>{}(42, 12), std::minus<double>{}(42, 12)}));
    }
}

TEST(Core_SignalSlot, Aggregate_Return_Values_Of_Slots_Into_A_Container) {
    Signal<double(double, double)> S;
    S.Connect(std::multiplies<double>{});
    S.Connect(std::plus<double>{});
    S.Connect(std::minus<double>{});
    
    auto Result = S.Aggregate<std::vector<double>>(42.0, 12.0);
    EXPECT_TRUE(Result == (std::vector<double>{std::multiplies<double>{}(42, 12), std::plus<double>{}(42, 12), std::minus<double>{}(42, 12) }));
}

TEST(Core_SignalSlot, RValue_LValue_Using_Accumulate) {
    Signal<int(std::shared_ptr<std::string>)> S;
    S.Connect([](std::shared_ptr<std::string> Str) {
        return Str == nullptr ? 0 : (int)Str->size();
    });
    S.Connect([](std::shared_ptr<std::string>&& Str) {
        return Str == nullptr ? 0 : (int)Str->size();
    });
    S.Connect([](std::shared_ptr<std::string> const& Str) {
        return Str == nullptr ? 0 : (int)Str->size();
    });
    auto TheAccumulator = S.Accumulate(0, std::plus<int>{});
    auto Ptr = std::make_shared<std::string>("test");
    EXPECT_TRUE(12 == TheAccumulator(Ptr));
}