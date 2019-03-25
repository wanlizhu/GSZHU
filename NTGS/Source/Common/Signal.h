#pragma once

#include <vector>       
#include <functional>   
#include <mutex>        
#include <memory>       
#include <algorithm>    
#include <cassert>      
#include <thread>       
#include <type_traits>  
#include <iterator>    

namespace NTGS {
    template<typename Policy, typename CallbackType>
    class BasicSignal;


    struct IDisconnector {
        virtual void operator()(size_t Index) const = 0;
    };


    inline void PseudoDeleter(IDisconnector*) {};


    class Connection {
        template<typename Policy, typename CallbackType>
        friend class BasicSignal;
    public:
        Connection() = default;
        Connection(const Connection&) = delete;
        Connection& operator=(const Connection&) = delete;
        Connection(Connection&& Conn) noexcept
            : mDisconnector(std::move(Conn.mDisconnector))
            , mIndex(Conn.mIndex) {}
        Connection& operator=(Connection&& Conn) noexcept {
            mDisconnector = std::move(Conn.mDisconnector);
            mIndex = Conn.mIndex;
            return *this;
        }

        inline bool IsConnected() const noexcept { return !mDisconnector.expired(); }
        inline void Disconnect() noexcept {
            auto SharedPointer = mDisconnector.lock();
            if (SharedPointer)
                (*SharedPointer)(mIndex);
            mDisconnector.reset();
        }

    private:
        Connection(const std::shared_ptr<IDisconnector>& Disconn, size_t Index)
            : mDisconnector(Disconn), mIndex(Index) {}

    private:
        std::weak_ptr<IDisconnector> mDisconnector;
        size_t mIndex = 0;
    };


    class ScopedConnection {
    public:
        ScopedConnection() = default;
        ScopedConnection(const ScopedConnection&) = delete;
        ScopedConnection& operator=(const ScopedConnection&) = delete;
        ScopedConnection(ScopedConnection&& Conn) noexcept : mConnection(std::move(Conn.mConnection)) {}
        ScopedConnection& operator=(ScopedConnection&& Conn) noexcept { Reset(std::move(Conn.mConnection)); return *this; }
        ScopedConnection(Connection&& Conn) : mConnection(std::forward<Connection>(Conn)) {}
        ScopedConnection& operator=(Connection&& Conn) { Reset(std::forward<Connection>(Conn)); return *this; }
        virtual ~ScopedConnection() { Disconnect(); }

        inline void Reset(Connection&& Conn = {}) noexcept { Disconnect(); mConnection = std::move(Conn); }
        inline Connection Detach() noexcept { auto Conn = std::move(mConnection); mConnection = Connection{}; return Conn; }
        inline bool IsConnected() const noexcept { return mConnection.IsConnected(); }
        inline void Disconnect() noexcept { mConnection.Disconnect(); }

    private:
        Connection mConnection;
    };


    struct MultiThreadPolicy {
        using MutexType = std::mutex;
        using MutexLockType = std::unique_lock<MutexType>;

        inline static void YieldThread() { std::this_thread::yield(); }
        inline static MutexLockType DeferLock(MutexType& Mutex) { return MutexLockType{ Mutex, std::defer_lock }; }
        inline static void Lock(MutexLockType& A, MutexLockType& B) { std::lock(A, B); }
    };


    struct SingleThreadPolicy {
        struct MutexType {};
        struct MutexLockType { explicit MutexLockType(const MutexType&) {} };

        inline static void YieldThread() {}
        inline static MutexLockType DeferLock(MutexType& Mutex) { return MutexLockType{ Mutex }; }
        inline static void Lock(MutexLockType&, MutexLockType&) {}
    };


    template<typename SignalType, typename T, typename FuncType, typename... ArgsType>
    class SignalAccumulator {
    public:
        using ResultType = typename std::result_of<FuncType(T, typename SignalType::SlotType::result_type)>::type;

        SignalAccumulator(const SignalType& SignalInst, T InitValue, FuncType Func)
            : mSignal(SignalInst), mInitValue(InitValue), mFunction(Func) {}

        ResultType operator()(ArgsType const& ... Args) const {
            return mSignal.TriggerAccumulation(mInitValue, mFunction, Args...);
        }

    private:
        const SignalType& mSignal;
        T mInitValue;
        FuncType mFunction;
    };


    template<typename Policy, typename ReturnType, typename... ArgsType>
    class BasicSignal<Policy, ReturnType(ArgsType...)> {
        template<typename, typename, typename, typename...>
        friend class SignalAccumulator;

        struct Disconnector : IDisconnector {
            BasicSignal<Policy, ReturnType(ArgsType...)>* mPointer = nullptr;

            Disconnector() : mPointer(nullptr) {}
            Disconnector(BasicSignal<Policy, ReturnType(ArgsType...)>* Pointer) : mPointer(Pointer) {}
            virtual void operator()(size_t Index) const override { if (mPointer) mPointer->Disconnect(Index); }
        };

    public:
        using ThreadPolicy = Policy;
        using MutexType = typename ThreadPolicy::MutexType;
        using MutexLockType = typename ThreadPolicy::MutexLockType;
        using SlotType = std::function<ReturnType(ArgsType...)>;
        using SizeType = typename std::vector<SlotType>::size_type;

        BasicSignal(const BasicSignal&) = delete;
        BasicSignal& operator=(const BasicSignal&) = delete;
        BasicSignal() : mSlotCount(0) {}
        BasicSignal(BasicSignal&& Sig) {
            MutexLockType LockInst{ Sig.mMutex };
            mSlotCount = std::move(Sig.mSlotCount);
            mSlots = std::move(Sig.mSlots);

            if (Sig.mDisconnInterface != nullptr) {
                mDisconnector = Disconnector{ this };
                mDisconnInterface = std::move(Sig.mDisconnInterface);
                *static_cast<Disconnector*>(mDisconnInterface.get()) = mDisconnector;
            }
        }
        BasicSignal& operator=(BasicSignal&& Sig) {
            auto Lock1 = ThreadPolicy::DeferLock(mMutex);
            auto Lock2 = ThreadPolicy::DeferLock(Sig.mMutex);
            ThreadPolicy::Lock(Lock1, Lock2);

            mSlotCount = std::move(Sig.mSlotCount);
            mSlots = std::move(Sig.mSlots);

            if (Sig.mDisconnInterface != nullptr) {
                mDisconnector = Disconnector{ this };
                mDisconnInterface = std::move(Sig.mDisconnInterface);
                *static_cast<Disconnector*>(mDisconnInterface.get()) = mDisconnector;
            }
            return *this;
        }
        virtual ~BasicSignal() { InvalidateDisconnector(); }

        template<typename T>
        inline Connection Connect(T&& Slot) {
            MutexLockType LockInst{ mMutex };

            mSlots.push_back(std::forward<T>(Slot));
            size_t Index = mSlots.size() - 1;

            if (mDisconnInterface == nullptr) {
                mDisconnector = Disconnector{ this };
                mDisconnInterface = std::shared_ptr<IDisconnector>{ &mDisconnector, PseudoDeleter };
            }

            ++mSlotCount;
            return Connection{ mDisconnInterface, Index };
        }

        inline void operator()(ArgsType const&... Args) const {
            for (const auto& Slot : CopySlots()) {
                if (Slot)
                    Slot(Args...);
            }
        }

        template<typename T, typename FuncType>
        inline SignalAccumulator<BasicSignal, T, FuncType, ArgsType...>
            Accumulate(T InitValue, FuncType Op) const {
            static_assert(std::is_same<ReturnType, void>::value == false,
                "Unable to accumulate slot return values with 'void' as return type.");
            return { *this, InitValue, Op };
        }

        template<typename ContainerType>
        inline ContainerType Aggregate(ArgsType const&... Args) const {
            ContainerType Container;

            auto Iterator = std::back_inserter(Container);
            for (const auto& Slot : CopySlots()) {
                if (Slot)
                    (*Iterator) = Slot(Args...);
            }

            return Container;
        }

        inline SizeType SlotCount() const { return mSlotCount; }
        inline bool IsEmpty() const { return SlotCount() == 0; }
        inline void DisconnectAll() {
            MutexLockType LockInst{ mMutex };
            mSlots.clear();
            mSlotCount = 0;
            InvalidateDisconnector();
        }

    private:
        void InvalidateDisconnector() {
            std::weak_ptr<IDisconnector> WeakPointer{ mDisconnInterface };
            mDisconnInterface.reset();

            while (WeakPointer.lock() != nullptr) {
                // We just yield here, allowing the OS to reschedule. 
                // We do this until all threads has released the disconnector object.
                ThreadPolicy::YieldThread();
            }
        }

        std::vector<SlotType> CopySlots() const {
            MutexLockType LockInst{ mMutex };
            return mSlots;
        }

        template<typename T, typename FuncType>
        typename SignalAccumulator<BasicSignal, T, FuncType, ArgsType...>::ResultType
            TriggerAccumulation(T Value, FuncType& Func, ArgsType const&... Args) const {
            for (const auto& Slot : CopySlots()) {
                if (Slot)
                    Value = Func(Value, Slot(Args...));
            }
            return Value;
        }

        void Disconnect(size_t Index) {
            MutexLockType LockInst{ mMutex };
            assert(mSlots.size() > Index);

            if (mSlots[Index] != nullptr)
                mSlotCount--;

            mSlots[Index] = SlotType{};
            while (mSlots.size() > 0 && !mSlots.back()) {
                mSlots.pop_back();
            }
        }

    private:
        mutable MutexType mMutex;
        std::vector<SlotType> mSlots;
        SizeType mSlotCount = 0;
        Disconnector mDisconnector;
        std::shared_ptr<IDisconnector> mDisconnInterface;
    };

    template<typename CallbackType>
    using Signal = BasicSignal<MultiThreadPolicy, CallbackType>;

    template<typename CallbackType>
    using UnsafeSignal = BasicSignal<SingleThreadPolicy, CallbackType>;

}