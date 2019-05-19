#pragma once

#include <GSZHU/BasicTypes.h>
#include <random>
#include <assert.h>

namespace GSZHU {
    template<typename T>
    void CombineHash(std::size_t* Seed, const T& Val) {
        *Seed ^= std::hash<T>()(Val) + 0x9e3779b9 + (*Seed << 6) + (*Seed >> 2);
    }

    template<typename FIRST, typename... REST>
    void CombineHash(std::size_t* Seed, const FIRST& First, const REST&... Rest) {
        CombineHash(Seed, First);
        CombineHash(Seed, Rest...);
    }

    template<typename... ARGS>
    std::size_t ComputeHash(const ARGS&... Args) {
        std::size_t Seed = 0;
        CombineHash(&Seed, Args...);
        return Seed;
    }

    template<typename T>
    std::size_t ComputeArrayHash(const T* Array, size_t N) {
        assert(N > 0);
        
        std::size_t Hash = 0;
        for (int i = 0; i < N; i++) {
            std::size_t ElementHash = std::hash<T>()(Array[i]);
            if (Hash == 0)
                Hash = ElementHash;
            else
                CombineHash(&Hash, ElementHash);
        }

        return Hash;
    }

    std::string ToString(const std::wstring& WStr);
    std::wstring ToWString(const std::string& Str);
}