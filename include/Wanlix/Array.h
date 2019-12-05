#pragma once

#include "Configuration.h"

namespace Wanlix
{
    class String;

    template<typename T>
    class Array
    {
        struct PrivateData;
    public:
        using VisitFunc      = void(*)(T&);
        using ConstVisitFunc = void(*)(const T&);
        using SortComparator = bool(*)(const T&, const T&);

        Array();
        Array(int size);
        Array(const T& x, const T& y, const T& z = T(0), const T& w = T(0));
        
        void Append(const T& val);
        void Append(const Array<T>& arr);
        void Resize(int size);
        void Reserve(int capacity);
        void Erase(int start, int size = 1);
        void Insert(int index, const T& val);
        void MakeUnique();
        void Visit(VisitFunc func);
        void Sort(SortComparator comparator = nullptr);

        T* GetData();
        T& operator[](int i);
        Array<T>& operator+=(const T& val);
        Array<T>& operator+=(const Array<T>& arr);
        operator T*();

        bool IsEmpty() const;
        int  GetSize() const;
        int  GetCapacity() const;
        int  Find(const T& val) const;
        void Visit(ConstVisitFunc func) const;

        const T* GetData() const;
        const T& operator[](int i) const;
        Array<T> operator+(const T& val) const;
        Array<T> operator+(const Array<T>& arr) const;

        operator bool() const;
        operator const T*() const;

        bool operator==(const Array<T>& rhs) const;
        bool operator!=(const Array<T>& rhs) const;
        bool operator< (const Array<T>& rhs) const;
        bool operator<=(const Array<T>& rhs) const;
        bool operator> (const Array<T>& rhs) const;
        bool operator>=(const Array<T>& rhs) const;

    private:
        PrivateData* mData = nullptr;
    };

    WANLIX_API Array<int>;
    WANLIX_API Array<String>;

}