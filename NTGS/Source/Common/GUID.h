#pragma once

#include <functional>
#include <iostream>
#include <array>
#include <sstream>
#include <utility>
#include <iomanip>

namespace NTGS {
    class GUID {
        friend std::ostream& operator<<(std::ostream& os, const GUID& guid);

    public:
        static GUID Create();

        GUID();
        explicit GUID(const std::array<uint8_t, 16>& bytes);
        explicit GUID(std::array<uint8_t, 16>&& bytes);
        explicit GUID(const std::string& str);
        
        bool operator==(const GUID& other) const;
        bool operator!=(const GUID& other) const;
        bool operator<(const GUID& other) const;

        std::string GetStr() const;
        operator std::string() const;
        const std::array<uint8_t, 16>& GetBytes() const;
        void Swap(GUID& other);
        bool IsValid() const;

    private:
        void Zeroify();

    private:
        std::array<uint8_t, 16> mBytes;
    };
}

namespace std {
    template<>
    void swap<NTGS::GUID>(NTGS::GUID& id1, NTGS::GUID& id2) noexcept;

    template<>
    struct hash<NTGS::GUID> {
        std::size_t operator()(const NTGS::GUID& id) const {
            const uint64_t* p = reinterpret_cast<const uint64_t*>(id.GetBytes().data());
            std::size_t seed1 = std::hash<uint64_t>()(p[0]);
            std::size_t seed2 = std::hash<uint64_t>()(p[1]);
            return seed2 +0x9e3779b9 + (seed1 << 6) + (seed1 >> 2);
        }
    };
}