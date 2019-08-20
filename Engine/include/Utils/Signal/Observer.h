#pragma once

#include <forward_list>
#include <memory>

#include "./Function.h"
#include "./Mutex.h"

namespace ZHU
{
    template<typename Mutex = NoopMutex>
    class Observer
    {
        // Only Nano::Signal is allowed access
        template<typename, typename> friend class Signal;
        using Slot_Pair = std::pair<const DelegateKey, Observer*>;

    public:
        void disconnect_all()
        {
            std::unique_lock<Mutex> lock(m_mutex);

            auto iter = m_connections.cbegin();
            auto stop = m_connections.cend();

            while (iter != stop) {
                auto const& delegate = (*iter)->first;
                auto const& observer = (*iter)->second;
                std::advance(iter, 1);

                // Do not remove from this while iterating
                if (observer != this) {
                    // Remove the delegate from the observer
                    observer->remove(delegate, this);
                }
            }
            // Then remove all this connections
            m_connections.clear();
        }

        bool is_empty() const
        {
            std::unique_lock<Mutex> lock(m_mutex);
            return m_connections.empty();
        }

        int slots_count() const 
        {
            std::unique_lock<Mutex> lock(m_mutex);
            auto iter = m_connections.cbegin();
            auto stop = m_connections.cend();

            int count = 0;
            while (iter != stop) {
                ++count;
                std::advance(iter, 1);
            }
            return count;
        }

    protected:
        // Guideline #4: A base class destructor should be
        // either public and virtual, or protected and non-virtual.
        ~Observer()
        {
            disconnect_all();
        }

    private:
        void insert(DelegateKey const& key, Observer* observer)
        {
            std::unique_lock<Mutex> lock(m_mutex);
            m_connections.emplace_front(std::make_unique<Slot_Pair>(key, observer));
        }

        void remove(DelegateKey const& key, Observer*)
        {
            std::unique_lock<Mutex> lock(m_mutex);
            m_connections.remove_if([&key] (auto const& pair) {
                return pair->first == key;
            });
        }

        template<typename Function, typename... Uref>
        void for_each(Uref&&... args)
        {
            std::unique_lock<Mutex> lock(m_mutex);

            auto iter = m_connections.cbegin();
            auto stop = m_connections.cend();

            while (iter != stop) {
                auto const& delegate = (*iter)->first;
                std::advance(iter, 1);

                // Perfect forward and fire
                Function::bind(delegate)(std::forward<Uref>(args)...);
            }
        }

        template<typename Function, typename Accumulate, typename... Uref>
        void for_each_accumulate(Accumulate&& accumulate, Uref&&... args)
        {
            std::unique_lock<Mutex> lock(m_mutex);

            auto iter = m_connections.cbegin();
            auto stop = m_connections.cend();

            while (iter != stop) {
                auto const& delegate = (*iter)->first;
                std::advance(iter, 1);

                // Perfect forward, fire, and accumulate the return value
                accumulate(Function::bind(delegate)(std::forward<Uref>(args)...));
            }
        }

    private:
        std::forward_list<std::unique_ptr<Slot_Pair>> m_connections;
        mutable Mutex m_mutex;
    };

} // namespace ZHU ------------------------------------------------------------
