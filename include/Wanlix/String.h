#pragma once

#include "Configuration.h"
#include "Array.h"

namespace Wanlix
{

    class WANLIX_API String
    {
        struct PrivateData;
    public:
        String();
        String(int size);
        String(const char* str, int size = -1);

        void Append(const String& str);
        void Append(double num, const char* format = "%lf");
        void Append(long long num, const char* format = "%lld");
        void AppendPath(const char* str);
        void SetCaseSensitive(bool flag);
        void Resize(int size);
        void Reserve(int capacity);
        void Erase(int start, int size = -1);
        void Insert(int index, const char* str);
        void Replace(const char* oldstr, const char* newstr);

        char*   GetData();
        char&   operator[](int i);
        String& operator+=(const char* str);
        String& Lower();
        String& Upper();
        String& Trim(bool left = true, bool right = true);
        String& Join(const Array<String>& strs);
        operator char*();
        
        bool IsEmpty() const;
        bool StartsWith(const char* str) const;
        bool EndsWith(const char* str) const;
        int  GetSize() const;
        int  GetCapacity() const;
        int  Find(const char* str, int start = 0) const;
        int  FindFirstOf(const char* chars, int start = 0) const;
        int  FindFirstNotOf(const char* chars, int start = 0) const;
        int  FindLastOf(const char* chars, int start = 0) const;
        int  FindLastNotOf(const char* chars, int start = 0) const;

        const char*   GetData() const;
        const char&   operator[](int i) const;
        String        operator+(const char* str) const;
        String        Slice(int start, int size = -1) const;
        Array<int>    FindAll(const char* str, int start = 0) const;
        Array<String> Split(const char* chars = " ") const;

        operator bool() const;
        operator const char* () const;

        bool operator==(const String& str) const;
        bool operator!=(const String& str) const;
        bool operator< (const String& str) const;
        bool operator<=(const String& str) const;
        bool operator> (const String& str) const;
        bool operator>=(const String& str) const;

    private:
        PrivateData* mData = nullptr;
    };
}