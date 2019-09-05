#pragma once

#include <atomic>
#include <thread>
#include <cassert>

namespace GS
{
    class NoopMutex
    {
    public:
        constexpr bool lock() const { return true; }
        constexpr bool try_lock() const { return true; }
        constexpr void unlock() noexcept {}
        constexpr void lock_shared() {}
        constexpr bool try_lock_shared() const { return true; }
        constexpr void unlock_shared() {}
    };

    //------------------------------------------------------------------------------

    class RecursiveMutex
    {
    public:
        bool try_lock()
        {
            if (!m_lock_flag.test_and_set(std::memory_order_acquire)) {
                m_owner_thread_id.store(std::this_thread::get_id(), std::memory_order_release);
            }
            else if (m_owner_thread_id.load(std::memory_order_acquire) != std::this_thread::get_id()) {
                return false;
            }
            ++m_recursive_counter;
            return true;
        }

        void lock()
        {
            while (!try_lock()) {
                std::this_thread::yield();
            }
        }

        void unlock()
        {
            assert(m_owner_thread_id.load(std::memory_order_acquire) == std::this_thread::get_id());
            assert(m_recursive_counter > 0);

            if (--m_recursive_counter == 0) {
                m_owner_thread_id.store(std::thread::id(), std::memory_order_release);
                m_lock_flag.clear(std::memory_order_release);
            }
        }

    private:
        std::atomic<std::thread::id> m_owner_thread_id = std::thread::id();
        std::uint32_t m_recursive_counter = 0;
        std::atomic_flag m_lock_flag = ATOMIC_FLAG_INIT;
    };


} // namespace ZHU ------------------------------------------------------------
