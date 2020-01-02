#include "Wanlix/Common.h"

namespace Wanlix
{
    AABB::AABB()
        : mMin{ 0.f, 0.f, 0.f }
        , mMax{ 0.f, 0.f, 0.f }
    {}

    AABB::AABB(Float3 const& min, Float3 const& max)
        : mMin(min)
        , mMax(max)
    {}

    Float3& AABB::GetMin()
    {
        return mMin;
    }

    Float3& AABB::GetMax()
    {
        return mMax;
    }

    Float3 const& AABB::GetMin() const
    {
        return mMin;
    }

    Float3 const& AABB::GetMax() const
    {
        return mMax;
    }

    Bool AABB::operator==(AABB const& rhs) const
    {
        return mMin == rhs.mMin && mMax == rhs.mMax;
    }

    Bool AABB::operator!=(AABB const& rhs) const
    {
        return !(*this == rhs);
    }

    Bool AABB::Intersects(AABB const& box) const
    {
        for (int i = 0; i < 3; i++) {
            if (mMin[i] > box.mMax[i])
                return false;
            if (mMax[i] < box.mMin[i])
                return false;
        }
        return true;
    }

    Bool AABB::Contains(AABB const& box) const
    {
        return box.mMin <= mMin && box.mMax <= mMax;
    }

    void AABB::Extent(Float3 const& point)
    {
        for (int i = 0; i < 3; i++) {
            if (point[i] < mMin[i])
                mMin[i] = point[i];
            if (point[i] > mMax[i])
                mMax[i] = point[i];
        }
    }

    void AABB::Extent(AABB const& box)
    {
        Extent(box.mMin);
        Extent(box.mMax);
    }

    DataBlock::DataBlock(Pvoid data, Size size, Uint flags)
        : mData(data)
        , mSize(size)
        , mFlags(flags)
    {}

    DataBlock::DataBlock(DataBlock const& rhs)
    {
        Free();
        mData = new Byte[rhs.mSize];
        std::memmove(mData, rhs.mData, rhs.mSize);
        mSize = rhs.mSize;
        mFlags = rhs.mFlags;
        mFlags |= DataBlockFlags::IsArray;
        mFlags |= DataBlockFlags::IsOwner;
    }

    DataBlock::DataBlock(DataBlock&& rhs)
    {
        Free();
        mData = rhs.mData;
        mSize = rhs.mSize;
        mFlags = rhs.mFlags;

        rhs.mData = nullptr;
        rhs.mSize = 0;
        rhs.mFlags = 0;
    }

    DataBlock::~DataBlock()
    {
        Free();
    }

    DataBlock& DataBlock::operator=(DataBlock const& rhs)
    {
        Free();
        mData = new Byte[rhs.mSize];
        std::memmove(mData, rhs.mData, rhs.mSize);
        mSize = rhs.mSize;
        mFlags = rhs.mFlags;
        mFlags |= DataBlockFlags::IsArray;
        mFlags |= DataBlockFlags::IsOwner;
    }

    DataBlock& DataBlock::operator=(DataBlock&& rhs)
    {
        Free();
        mData = rhs.mData;
        mSize = rhs.mSize;
        mFlags = rhs.mFlags;

        rhs.mData = nullptr;
        rhs.mSize = 0;
        rhs.mFlags = 0;
    }

    void DataBlock::Free()
    {
        if ((mFlags & DataBlockFlags::IsOwner) != 0) {
            if (mData != nullptr) {
                if ((mFlags & DataBlockFlags::IsArray) != 0) {
                    delete[] mData;
                }
                else {
                    delete mData;
                }
                mData = nullptr;
            }
        }

        mSize = 0;
        mFlags = 0;
    }

    Byte& DataBlock::operator[](int index)
    {
        assert((Uint)index < mSize);
        return *((Byte*)mData + index);
    }

    Byte const& DataBlock::operator[](int index) const
    {
        assert((Uint)index < mSize);
        return *((const Byte*)mData + index);
    }

    Bool DataBlock::operator==(DataBlock const& rhs) const
    {
        if (mSize != rhs.mSize) {
            return false;
        }

        return std::memcmp(mData, rhs.mData, mSize) == 0;
    }

    Bool DataBlock::operator!=(DataBlock const& rhs) const
    {
        if (mSize != rhs.mSize) {
            return false;
        }

        return std::memcmp(mData, rhs.mData, mSize) == 0;
    }

    Size  DataBlock::GetSize() const
    {
        return mSize;
    }

    Uint  DataBlock::GetFlags() const
    {
        return mFlags;
    }

    void* DataBlock::GetData()
    {
        return mData;
    }

    void const* DataBlock::GetData() const
    {
        return mData;
    }
}