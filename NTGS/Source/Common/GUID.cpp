#include "Common/GUID.h"
#ifdef _WIN32
#include <objbase.h>
#endif

#ifdef APPLE
#include <CoreFoundation/CFUUID.h>
#endif

#ifdef LINUX
#include <uuid/uuid.h>
#endif

namespace NTGS {
    uint8_t HexToChar(char ch) {
        // 0-9
        if (ch > 47 && ch < 58)
            return ch - 48;

        // a-f
        if (ch > 96 && ch < 103)
            return ch - 87;

        // A-F
        if (ch > 64 && ch < 71)
            return ch - 55;

        return 0;
    }

    bool IsHexChar(char ch) {
        // 0-9
        if (ch > 47 && ch < 58)
            return true;

        // a-f
        if (ch > 96 && ch < 103)
            return true;

        // A-F
        if (ch > 64 && ch < 71)
            return true;

        return false;
    }

    uint8_t HexPairToChar(char a, char b) {
        return HexToChar(a) * 16 + HexToChar(b);
    }

    std::ostream& operator<<(std::ostream& os, const GUID& id) {
        std::ios_base::fmtflags f(os.flags());
        os << std::hex << std::setfill('0')
            << std::setw(2) << (int) id.mBytes[0]
            << std::setw(2) << (int) id.mBytes[1]
            << std::setw(2) << (int) id.mBytes[2]
            << std::setw(2) << (int) id.mBytes[3]
            << "-"
            << std::setw(2) << (int) id.mBytes[4]
            << std::setw(2) << (int) id.mBytes[5]
            << "-"
            << std::setw(2) << (int) id.mBytes[6]
            << std::setw(2) << (int) id.mBytes[7]
            << "-"
            << std::setw(2) << (int) id.mBytes[8]
            << std::setw(2) << (int) id.mBytes[9]
            << "-"
            << std::setw(2) << (int) id.mBytes[10]
            << std::setw(2) << (int) id.mBytes[11]
            << std::setw(2) << (int) id.mBytes[12]
            << std::setw(2) << (int) id.mBytes[13]
            << std::setw(2) << (int) id.mBytes[14]
            << std::setw(2) << (int) id.mBytes[15];
        os.flags(f);
        return os;
    }


#ifdef _WIN32
    GUID GUID::Create() {
        ::GUID id;
        CoCreateGuid(&id);

        std::array<unsigned char, 16> bytes = {
            (unsigned char) ((id.Data1 >> 24) & 0xFF),
            (unsigned char) ((id.Data1 >> 16) & 0xFF),
            (unsigned char) ((id.Data1 >> 8) & 0xFF),
            (unsigned char) ((id.Data1) & 0xff),

            (unsigned char) ((id.Data2 >> 8) & 0xFF),
            (unsigned char) ((id.Data2) & 0xff),

            (unsigned char) ((id.Data3 >> 8) & 0xFF),
            (unsigned char) ((id.Data3) & 0xFF),

            (unsigned char) id.Data4[0],
            (unsigned char) id.Data4[1],
            (unsigned char) id.Data4[2],
            (unsigned char) id.Data4[3],
            (unsigned char) id.Data4[4],
            (unsigned char) id.Data4[5],
            (unsigned char) id.Data4[6],
            (unsigned char) id.Data4[7]
        };

        return GUID{ std::move(bytes) };
    }
#endif

#ifdef APPLE
    GUID GUID::Create() {
        auto id = CFUUIDCreate(NULL);
        auto bytes = CFUUIDGetUUIDBytes(id);
        CFRelease(id);

        std::array<unsigned char, 16> byteArray = { {
            bytes.byte0,
            bytes.byte1,
            bytes.byte2,
            bytes.byte3,
            bytes.byte4,
            bytes.byte5,
            bytes.byte6,
            bytes.byte7,
            bytes.byte8,
            bytes.byte9,
            bytes.byte10,
            bytes.byte11,
            bytes.byte12,
            bytes.byte13,
            bytes.byte14,
            bytes.byte15
        } };
        return GUID{ std::move(byteArray) };
    }
#endif

#ifdef LINUX
    GUID GUID::Create() {
        std::array<unsigned char, 16> data;
        static_assert(std::is_same<unsigned char[16], uuid_t>::value, "Wrong type!");
        uuid_generate(data.data());
        return GUID{ std::move(data) };
    }
#endif

    GUID::GUID()
        : mBytes({ 0 }) {}

    GUID::GUID(const std::array<uint8_t, 16>& bytes)
        : mBytes(bytes) {}

    GUID::GUID(std::array<uint8_t, 16>&& bytes)
        : mBytes(std::move(bytes)) {}

    GUID::GUID(const std::string& str) {
        char char1 = '\0';
        char char2 = '\0';
        bool isFirst = true;
        unsigned nextByte = 0;

        for (const char &ch : str) {
            if (ch == '-')
                continue;

            if (nextByte >= 16 || !IsHexChar(ch)) {
                Zeroify();
                return;
            }

            if (isFirst) {
                char1 = ch;
                isFirst = false;
            }
            else {
                char2 = ch;
                auto byte = HexPairToChar(char1, char2);
                mBytes[nextByte++] = byte;
                isFirst = true;
            }
        }

        if (nextByte < 16) {
            Zeroify();
            return;
        }
    }

    bool GUID::operator==(const GUID& other) const {
        return mBytes == other.mBytes;
    }

    bool GUID::operator!=(const GUID& other) const {
        return !(*this == other);
    }

    bool GUID::operator<(const GUID& other) const {
        return mBytes < other.mBytes;
    }

    std::string GUID::GetStr() const {
        char one[10], two[6], three[6], four[6], five[14];

        snprintf(one, 10, "%02x%02x%02x%02x",
            mBytes[0], mBytes[1], mBytes[2], mBytes[3]);
        snprintf(two, 6, "%02x%02x",
            mBytes[4], mBytes[5]);
        snprintf(three, 6, "%02x%02x",
            mBytes[6], mBytes[7]);
        snprintf(four, 6, "%02x%02x",
            mBytes[8], mBytes[9]);
        snprintf(five, 14, "%02x%02x%02x%02x%02x%02x",
            mBytes[10], mBytes[11], mBytes[12], mBytes[13], mBytes[14], mBytes[15]);
        const std::string sep("-");
        std::string out(one);

        out += sep + two;
        out += sep + three;
        out += sep + four;
        out += sep + five;

        return out;
    }

    GUID::operator std::string() const {
        return GetStr();
    }

    const std::array<uint8_t, 16>& GUID::GetBytes() const {
        return mBytes;
    }

    void GUID::Swap(GUID& other) {
        mBytes.swap(other.mBytes);
    }

    bool GUID::IsValid() const {
        GUID empty;
        return *this != empty;
    }

    void GUID::Zeroify() {
        std::fill(mBytes.begin(), mBytes.end(), 0);
    }
}

namespace std {
    template<>
    void swap<NTGS::GUID>(NTGS::GUID& id1, NTGS::GUID& id2) noexcept {
        id1.Swap(id2);
    }
}