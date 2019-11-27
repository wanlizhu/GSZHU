#pragma once

#include <queue>
#include <vector>
#include <algorithm>

namespace Wanlix
{
    template<typename T,
             typename _Container_ = std::vector<T>,
             typename _Pred_ = std::less<typename _Container_::value_type>>
    class IterablePriorityQueue
    {
    public:
        using container_type  = _Container_;
        using value_compare   = _Pred_;
        using value_type      = typename _Container_::value_type;
        using size_type       = typename _Container_::size_type;
        using reference       = typename _Container_::reference;
        using const_reference = typename _Container_::const_reference;
        using const_iterator  = typename _Container_::const_iterator;

        static_assert(std::is_same_v<T, value_type>, "container adaptors require consistent types");

        IterablePriorityQueue() noexcept(
            std::is_nothrow_default_constructible_v<_Container_> && 
            std::is_nothrow_default_constructible_v<value_compare>
        )
            : mContainer(),
            , mPredicate()
        {}

        explicit IterablePriorityQueue(const _Pred_& pred) noexcept(
            std::is_nothrow_default_constructible_v<_Container_>&&
            std::is_nothrow_default_constructible_v<value_compare>
        )
            : mContainer(),
            , mPredicate(pred)
        {}

        IterablePriorityQueue(
            const _Pred_& pred,
            const _Container_& container
        )
            : mContainer(container)
            , mPredicate(pred)
        {
            std::make_heap(mContainer.begin(), mContainer.end(), mPredicate);
        }

        template<typename _It_>
        IterablePriorityQueue(
            _It_ first,
            _It_ last
        )
            : mContainer(first, last)
            , mPredicate()
        {
            std::make_heap(mContainer.begin(), mContainer.end(), mPredicate);
        }

        template<typename _It_>
        IterablePriorityQueue(
            _It_ first,
            _It_ last,
            const _Pred_& pred
        )
            : mContainer(first, last)
            , mPredicate(pred)
        {
            std::make_heap(mContainer.begin(), mContainer.end(), mPredicate);
        }

        IterablePriorityQueue(
            const _Pred_& pred,
            _Container_&& container
        )
            : mContainer(std::move(container))
            , mPredicate(pred)
        {}

        void push(value_type&& val) {
            mContainer.push_back(std::move(val));
            std::push_heap(mContainer.begin(), mContainer.end(), mPredicate);
        }

        template<typename... _Args_>
        void emplace(_Args_&&... args) {
            mContainer.emplace_back(std::forward<_Args_>(args)...);
            std::push_heap(mContainer.begin(), mContainer.end(), mPredicate);
        }

        [[nodiscard]] bool empty() const {
            return mContainer.empty();
        }

        [[nodiscard]] size_type size() const {
            return mContainer.size();
        }

        [[nodiscard]] const_reference top() const {
            return mContainer.front();
        }

        [[nodiscard]] void push(const value_type& val) {
            mContainer.push_back(val);
            std::push_heap(mContainer.begin(), mContainer.end(), mPredicate);
        }

        void pop() {
            std::pop_heap(mContainer.begin(), mContainer.end(), mPredicate);
            mContainer.pop_back();
        }

        void swap(IterablePriorityQueue& other) noexcept(
                std::is_nothrow_swappable_v<_Container_> &&
                std::is_nothrow_swappable_v<_Pred_>
            )
        {
            std::swap(mContainer, other.mContainer);
            std::swap(mPredicate, other.mPredicate);
        }

        constexpr const_iterator cbegin() const noexcept {
            return mContainer.begin();
        }

        constexpr const_iterator cend() const noexcept {
            return mContainer.end();
        }

        constexpr const_reference operator[](size_type pos) const {
            return mContainer[pos];
        }

        void erase(const_iterator it) {
            mContainer.erase(it);
            std::make_heap(mContainer.begin(), mContainer.end(), mPredicate);
        }

    protected:
        _Container_ mContainer;
        _Pred_ mPredicate;
    };
}
