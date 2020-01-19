#pragma once

#include <vector>
#include <algorithm>
#include <type_traits>
#include <memory>

namespace Wanlix
{
    template<typename T, class _Pred_ = std::less<T>>
    class OrderedList
    {
    public:
        using container_type  = std::vector<T>;
        using value_compare   = _Pred_;
        using value_type      = typename container_type::value_type;
        using size_type       = typename container_type::size_type;
        using reference       = typename container_type::reference;
        using const_reference = typename container_type::const_reference;
        using iterator        = typename container_type::iterator;
        using const_iterator  = typename container_type::const_iterator;

        static_assert(std::is_same_v<T, value_type>);

        inline OrderedList() noexcept 
        {}
        inline OrderedList(const value_compare& comp) noexcept
            : m_compare(comp)
        {}
        inline OrderedList(const container_type& container, const value_compare& comp) noexcept
            : m_container(container)
            , m_compare(comp)
        {
            std::make_heap(m_container.begin(), m_container.end(), m_compare);
        }

        inline void push_back(value_type&& val) noexcept {
            m_container.push_back(std::move(val));
            std::push_heap(m_container.begin(), m_container.end(), m_compare);
        }

        template<typename... _Args_>
        inline void emplace_back(_Args_&& ... args) noexcept {
            m_container.emplace_back(std::forward<_Args_>(args)...);
            std::push_heap(m_container.begin(), m_container.end(), m_compare);
        }

        inline void pop() noexcept { // erase highest-priority element
            std::pop_heap(m_container.begin(), m_container.end(), m_compare);
            m_container.pop_back();
        }

        inline iterator erase(iterator it) noexcept {
            auto result = m_container.erase(it);
            std::make_heap(m_container.begin(), m_container.end(), m_compare);
            return result;
        }

        inline iterator erase(const_iterator it) noexcept {
            auto result = m_container.erase(it);
            std::make_heap(m_container.begin(), m_container.end(), m_compare);
            return result;
        }

        inline const_reference top() const noexcept { return m_container.front(); }
        inline reference top() noexcept { return m_container.front(); }
        inline bool empty() const noexcept { return m_container.empty(); }
        inline size_type size() const noexcept { return m_container.size(); }
        inline const_iterator begin() const noexcept { return m_container.begin(); }
        inline iterator begin() noexcept { return m_container.begin(); }
        inline const_iterator end() const noexcept { return m_container.end(); }
        inline iterator end() noexcept { return m_container.end(); }

    protected:
        container_type m_container;
        value_compare  m_compare;
    };
}