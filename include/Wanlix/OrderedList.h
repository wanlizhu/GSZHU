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

        OrderedList() {}
        OrderedList(value_compare const& comp)
            : m_compare(comp)
        {}
        OrderedList(container_type const& container, value_compare const& comp)
            : m_container(container)
            , m_compare(comp)
        {
            std::make_heap(m_container.begin(), m_container.end(), m_compare);
        }

        void push_back(value_type&& val) { 
            m_container.push_back(std::move(val));
            std::push_heap(m_container.begin(), m_container.end(), m_compare);
        }

        template<typename... _Args_>
        void emplace_back(_Args_&& ... args) { 
            m_container.emplace_back(std::forward<_Args_>(args)...);
            std::push_heap(m_container.begin(), m_container.end(), m_compare);
        }

        void pop() { // erase highest-priority element
            std::pop_heap(m_container.begin(), m_container.end(), m_compare);
            m_container.pop_back();
        }

        iterator erase(iterator it) {
            auto result = m_container.erase(it);
            std::make_heap(m_container.begin(), m_container.end(), m_compare);
            return result;
        }

        iterator erase(const_iterator it) {
            auto result = m_container.erase(it);
            std::make_heap(m_container.begin(), m_container.end(), m_compare);
            return result;
        }

        const_reference top() const { return m_container.front(); }
        reference top() { return m_container.front(); }
        bool empty() const { return m_container.empty(); }
        size_type size() const { return m_container.size(); }
        const_iterator begin() const { return m_container.begin(); }
        iterator begin() { return m_container.begin(); }
        const_iterator end() const { return m_container.end(); }
        iterator end() { return m_container.end(); }

    protected:
        container_type m_container;
        value_compare  m_compare;
    };
}