#include <gtest/gtest.h>
#include "Common/GUID.h"
#include <unordered_map>
#include <map>

using namespace NTGS;

TEST(NTGS_Common, GUID_TEST) {
    GUID r1 = GUID::Create();
    GUID r2 = GUID::Create();
    GUID r3 = GUID::Create();

    GUID s1("7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");
    GUID s2("16d1bd03-09a5-47d3-944b-5e326fd52d27");
    GUID s3("fdaba646-e07e-49de-9529-4499a5580c75");
    GUID s4("7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");
    GUID s5("7bcd757f-5b10-4f9b-af69-1a1f226f3b31");

    EXPECT_FALSE(r1 == r2 || r1 == r3 || r2 == r3);
    EXPECT_TRUE(s1 != s2);
    EXPECT_TRUE(s1 == s4);
    EXPECT_FALSE(s4 < s5);
    EXPECT_FALSE(s4 == s5);

    std::stringstream ss1;
    ss1 << s1;
    EXPECT_STREQ(ss1.str().c_str(), "7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");
    EXPECT_STREQ(s1.GetStr().c_str(), "7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");

    std::stringstream ss2;
    ss2 << s2;
    EXPECT_STREQ(ss2.str().c_str(), "16d1bd03-09a5-47d3-944b-5e326fd52d27");
    EXPECT_STREQ(s2.GetStr().c_str(), "16d1bd03-09a5-47d3-944b-5e326fd52d27");

    auto swap1 = GUID::Create();
    auto swap2 = GUID::Create();
    auto swap3 = swap1;
    auto swap4 = swap2;
    EXPECT_TRUE(swap1 == swap3 && swap2 == swap4);
    EXPECT_FALSE(swap1 == swap2);

    {
        std::unordered_map<GUID, int> m = { {s1, 1}, {s2, 2} };
        auto it1 = m.find(s1);
        auto it2 = m.find(s2);
        auto it3 = m.find(s3);

        EXPECT_FALSE(it1 == m.end() || it2 == m.end());
        EXPECT_TRUE(it1->second == 1 && it2->second == 2);
        EXPECT_TRUE(it3 == m.end());
    }

    std::array<unsigned char, 16> bytes = { {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xdd
    } };

    GUID idFromBytes(bytes);
    GUID idFromStr("0102030405060708090a0b0c0d0e0fdd");
    EXPECT_EQ(idFromBytes, idFromStr);
}