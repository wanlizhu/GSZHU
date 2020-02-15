#pragma once

#include <future>
#include <optional>

namespace Wanli
{
    template<typename T>
    class Future
    {
	public:
		Future() noexcept = default;

		Future(std::future<T> && future) noexcept :
			mFuture(std::move(future))
		{}

		inline bool HasValue() const noexcept 
		{
			return mFuture.valid() || mCurrent;
		}

		inline T& Get() noexcept 
		{
			if (mFuture.valid()) 
			{
				mCurrent = mFuture.get();
			}

			return *mCurrent;
		}

		constexpr explicit operator bool() const noexcept { return HasValue(); }
		constexpr operator T& () const noexcept { return *Get(); }

		T& operator*() noexcept { return Get(); }
		T& operator->() noexcept { return Get(); }

		bool operator==(const Future & other) const noexcept 
		{
			return mFuture == other.mFuture && mCurrent == other.mCurrent;
		}

		bool operator!=(const Future & other) const noexcept
		{
			return !operator==(other);
		}

	private:
		std::future<T> mFuture;
		std::optional<T> mCurrent;
    };
}