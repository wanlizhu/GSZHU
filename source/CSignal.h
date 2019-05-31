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
#include <unordered_map>

namespace GSZHU {
    template<typename MUTEX_LOCKER, typename CALLBACK>
    class CSignal;

    class IDisconnector {
    public:
        virtual void Disconnect(size_t conn) = 0;
    };

    class CConection {
        template<typename MUTEX_LOCKER, typename CALLBACK>
        friend class CSignal;
    public:
        CConection() = default;
        CConection(const CConection&) = delete;
        CConection& operator=(const CConection&) = delete;

        inline size_t GetID() const noexcept {
            return mConnectionID;
        }
        inline bool IsConnected() const noexcept {
            return !mDisconnector.expired() && mConnectionID != -1;
        }
        inline void Disconnect() noexcept {
            auto SharedPointer = mDisconnector.lock();
            if (SharedPointer) {
                SharedPointer->Disconnect(mConnectionID);
            }
            mDisconnector.reset();
            mConnectionID = -1;
        }

    private:
        CConection(std::weak_ptr<IDisconnector> disconn, size_t conn)
            : mDisconnector(disconn)
            , mConnectionID(conn)
        {}

    private:
        std::weak_ptr<IDisconnector> mDisconnector;
        size_t mConnectionID = -1;
    };

    struct PSEUDO_LOCKER {
        PSEUDO_LOCKER(const std::mutex&) {}
    };

    using STD_LOCKER = std::lock_guard<std::mutex>;

    template<typename MUTEX_LOCKER, typename RET, typename... ARGS>
    class CSignal<MUTEX_LOCKER, RET(ARGS...)> : public IDisconnector {
    public:
        using Type = CSignal<MUTEX_LOCKER, RET(ARGS...)>;
        using SLOT_FUNC = std::function<RET(ARGS...)>;

        CSignal(const CSignal&) = delete;
        CSignal& operator=(const CSignal&) = delete;
        CSignal() = default;
        virtual ~CSignal() {}

        template<typename T>
        inline CConection Connect(T&& slot) {
            MUTEX_LOCKER Locker(mMutex);

            if (!mShadow)
                mShadow = std::shared_ptr<CSignal>(this, [](Type*) {}); // Pseudo-delete

            size_t ID = (char*)&slot - (char*)0;
            mSlotTable[ID] = std::forward<T>(slot);

            return CConection(mShadow, ID);
        }

        inline void operator()(ARGS const&... args) const {
            std::vector<SLOT_FUNC> Slots(mSlotTable.size());
            {
                MUTEX_LOCKER Locker(mMutex);
                for (const auto& Value : mSlotTable)
                    Slots.push_back(Value.second);
            }

            for (auto& slot : Slots) {
                if (slot) slot(args...);
            }
        }

        virtual void Disconnect(size_t conn) override {
            MUTEX_LOCKER Locker(mMutex);

            auto Iter = mSlotTable.find(conn);
            if (Iter != mSlotTable.end()) {
                mSlotTable.erase(Iter);
            }
        }

        inline void DisconnectAll() {
            MUTEX_LOCKER Locker(mMutex);
            mSlotTable.clear();
        }

        inline size_t SlotCount() const noexcept { return mSlotTable.size(); }
        inline bool IsEmpty() const noexcept { return mSlotTable.empty(); }

    private:
        mutable std::mutex mMutex;
        std::shared_ptr<CSignal> mShadow;
        std::unordered_map<size_t, SLOT_FUNC> mSlotTable;
    };

    template<typename RET, typename... ARGS>
    using CSignal_ST = CSignal<PSEUDO_LOCKER, RET(ARGS...)>;

    template<typename RET, typename... ARGS>
    using CSignal_MT = CSignal<STD_LOCKER, RET(ARGS...)>;
}