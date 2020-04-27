#include<gtest/gtest.h>
#include "dejavu/core/strings.h"

TEST(core_string, ichar_traits_test)
{
    using istring = std::basic_string<char, djv::ichar_traits<char>>;
    using iwstring = std::basic_string<wchar_t, djv::ichar_traits<wchar_t>>;

    istring str1 = "abcd123ABCD";
    istring str11 = "ABCD123abcd";
    EXPECT_EQ(str1, str11);
    EXPECT_EQ(str1.find("ABCD"), 0);
    EXPECT_EQ(str11.find("abcd"), 0);

    iwstring str2 = L"abcd123ABCD";
    iwstring str21 = L"ABCD123abcd";
    EXPECT_EQ(str2, str21);
    EXPECT_EQ(str2.find(L"ABCD"), 0);
    EXPECT_EQ(str21.find(L"abcd"), 0);
}

TEST(core_string, trim_inplace_test)
{
    std::string str = " \t \r \n ABCD \t\r\n";
    djv::trim_inplace(str);
    EXPECT_EQ(str, "ABCD");

    std::wstring wstr = L" \t \r \n ABCD \t\r\n";
    djv::trim_inplace(wstr);
    EXPECT_EQ(wstr, L"ABCD");
}

TEST(core_string, tolower_inplace_test)
{
    std::string str = "ABCD123";
    djv::tolower_inplace(str);
    EXPECT_EQ(str, "abcd123");

    std::wstring wstr = L"ABCD123";
    djv::tolower_inplace(wstr);
    EXPECT_EQ(wstr, L"abcd123");
}

TEST(core_string, toupper_inplace_test)
{
    std::string str = "abcd123";
    djv::toupper_inplace(str);
    EXPECT_EQ(str, "ABCD123");

    std::wstring wstr = L"abcd123";
    djv::toupper_inplace(wstr);
    EXPECT_EQ(wstr, L"ABCD123");
}

TEST(core_string, starts_with_test)
{
    std::string str = "abcd123xyz";
    EXPECT_TRUE(djv::starts_with(str, std::string("abcd")));
    EXPECT_TRUE(djv::starts_with(str, "abcd"));

    std::wstring wstr = L"abcd123xyz";
    EXPECT_TRUE(djv::starts_with(wstr, std::wstring(L"abcd")));
    EXPECT_TRUE(djv::starts_with(wstr, L"abcd"));
}

TEST(core_string, istring_starts_with_test)
{
    using istring = std::basic_string<char, djv::ichar_traits<char>>;
    using iwstring = std::basic_string<wchar_t, djv::ichar_traits<wchar_t>>;

    istring str1 = "a.b.c.d123xyz";
    EXPECT_TRUE(djv::starts_with(str1, istring("A.B.C.D")));
    EXPECT_TRUE(djv::starts_with(str1, "A.B.C.D"));

    iwstring wstr1 = L"a.b.c.d123xyz";
    EXPECT_TRUE(djv::starts_with(wstr1, iwstring(L"A.B.C.D")));
    EXPECT_TRUE(djv::starts_with(wstr1, L"A.B.C.D"));
}

TEST(core_string, ends_with_test)
{
    std::string str = "abcd123xyz";
    EXPECT_TRUE(djv::ends_with(str, std::string("xyz")));
    EXPECT_TRUE(djv::ends_with(str, "xyz"));

    std::wstring wstr = L"abcd123xyz";
    EXPECT_TRUE(djv::ends_with(wstr, std::wstring(L"xyz")));
    EXPECT_TRUE(djv::ends_with(wstr, L"xyz"));
}

TEST(core_string, istring_ends_with_test)
{
    using istring = std::basic_string<char, djv::ichar_traits<char>>;
    using iwstring = std::basic_string<wchar_t, djv::ichar_traits<wchar_t>>;

    istring str1 = "abcd123x.y.z";
    EXPECT_TRUE(djv::ends_with(str1, istring("X.Y.Z")));
    EXPECT_TRUE(djv::ends_with(str1, "X.Y.Z"));

    iwstring wstr1 = L"abcd123x.y.z";
    EXPECT_TRUE(djv::ends_with(wstr1, iwstring(L"X.Y.Z")));
    EXPECT_TRUE(djv::ends_with(wstr1, L"X.Y.Z"));
}

TEST(core_string, starts_of_test)
{
    std::string str = "abcd123xyz";
    EXPECT_TRUE(djv::starts_of(str, std::string("-aefg")));
    EXPECT_TRUE(djv::starts_of(str, "-aefg"));
    EXPECT_FALSE(djv::starts_of(str, "-Aefg"));

    std::wstring wstr = L"abcd123xyz";
    EXPECT_TRUE(djv::starts_of(wstr, std::wstring(L"-aefg")));
    EXPECT_TRUE(djv::starts_of(wstr, L"-aefg"));
    EXPECT_FALSE(djv::starts_of(wstr, L"-Aefg"));
}

TEST(core_string, istring_starts_of_test)
{
    using istring = std::basic_string<char, djv::ichar_traits<char>>;
    using iwstring = std::basic_string<wchar_t, djv::ichar_traits<wchar_t>>;

    istring str1 = "a.b.c.d123xyz";
    EXPECT_TRUE(djv::starts_of(str1, istring("-AEFG")));
    EXPECT_TRUE(djv::starts_of(str1, "-AEFG"));

    iwstring wstr1 = L"a.b.c.d123xyz";
    EXPECT_TRUE(djv::starts_of(wstr1, iwstring(L"-AEFG")));
    EXPECT_TRUE(djv::starts_of(wstr1, L"-AEFG"));
}

TEST(core_string, ends_of_test)
{
    std::string str = "abcd123xyz";
    EXPECT_TRUE(djv::ends_of(str, std::string("zabcd")));
    EXPECT_TRUE(djv::ends_of(str, "zabcd"));
    EXPECT_FALSE(djv::ends_of(str, "Zabcd"));

    std::wstring wstr = L"abcd123xyz";
    EXPECT_TRUE(djv::ends_of(wstr, std::wstring(L"zabcd")));
    EXPECT_TRUE(djv::ends_of(wstr, L"zabcd"));
    EXPECT_FALSE(djv::ends_of(wstr, L"Zabcd"));
}

TEST(core_string, istring_ends_of_test)
{
    using istring = std::basic_string<char, djv::ichar_traits<char>>;
    using iwstring = std::basic_string<wchar_t, djv::ichar_traits<wchar_t>>;

    istring str1 = "abcd123x.y.z";
    EXPECT_TRUE(djv::ends_of(str1, istring("Zabcd")));
    EXPECT_TRUE(djv::ends_of(str1, "Zabcd"));

    iwstring wstr1 = L"abcd123x.y.z";
    EXPECT_TRUE(djv::ends_of(wstr1, iwstring(L"Zabcd")));
    EXPECT_TRUE(djv::ends_of(wstr1, L"Zabcd"));
}

TEST(core_string, istrcmp_test)
{
    std::string str1 = "abcd123ABCD";
    std::string str11 = "ABCD123abcd";
    EXPECT_TRUE(djv::istrcmp(str1, str11) == 0);
    EXPECT_TRUE(djv::istrcmp(str1, "ABCD123abcd") == 0);
    EXPECT_TRUE(djv::istrcmp(str1, 4, 3, "123") == 0);
    EXPECT_TRUE(djv::istrcmp(str1, 4, 3, "xyz123xyz", 3) == 0);

    std::wstring str2 = L"abcd123ABCD";
    std::wstring str21 = L"ABCD123abcd";
    EXPECT_TRUE(djv::istrcmp(str2, str21) == 0);
    EXPECT_TRUE(djv::istrcmp(str2, L"ABCD123abcd") == 0);
    EXPECT_TRUE(djv::istrcmp(str2, 4, 3, L"123") == 0);
    EXPECT_TRUE(djv::istrcmp(str2, 4, 3, L"xyz123xyz", 3) == 0);
}

TEST(core_string, replace_inplace_test)
{
    std::string str1 = "abcd123ABCD";
    djv::replace_inplace(str1, "ab", "XX");
    EXPECT_EQ(str1, "XXcd123ABCD");

    str1 = "abcd123ABCD";
    djv::replace_inplace(str1, "ab", "XXXX");
    EXPECT_EQ(str1, "XXXXcd123ABCD");

    str1 = "abcd123ABCD";
    djv::replace_inplace(str1, "123", "***");
    EXPECT_EQ(str1, "abcd***ABCD");

    str1 = "abcd123ABCD";
    djv::replace_inplace(str1, "123", "*");
    EXPECT_EQ(str1, "abcd*ABCD");

    std::wstring str2 = L"abcd123ABCD";
    djv::replace_inplace(str2, L"ab", L"XX");
    EXPECT_EQ(str2, L"XXcd123ABCD");

    str2 = L"abcd123ABCD";
    djv::replace_inplace(str2, L"ab", L"XXXX");
    EXPECT_EQ(str2, L"XXXXcd123ABCD");

    str2 = L"abcd123ABCD";
    djv::replace_inplace(str2, L"123", L"***");
    EXPECT_EQ(str2, L"abcd***ABCD");

    str2 = L"abcd123ABCD";
    djv::replace_inplace(str2, L"123", L"*");
    EXPECT_EQ(str2, L"abcd*ABCD");
}

TEST(core_string, istring_replace_inplace_test)
{
    using istring = std::basic_string<char, djv::ichar_traits<char>>;
    using iwstring = std::basic_string<wchar_t, djv::ichar_traits<wchar_t>>;

    istring str1 = "abcd123ABCD";
    djv::replace_inplace(str1, "ab", "XX");
    EXPECT_EQ(str1, "XXcd123XXCD");

    str1 = "abcd123ABCD";
    djv::replace_inplace(str1, "ab", "XXXX");
    EXPECT_EQ(str1, "XXXXcd123XXXXCD");

    str1 = "abcd123ABCD";
    djv::replace_inplace(str1, "123", "***");
    EXPECT_EQ(str1, "abcd***ABCD");

    str1 = "abcd123ABCD";
    djv::replace_inplace(str1, "123", "*");
    EXPECT_EQ(str1, "abcd*ABCD");

    iwstring str2 = L"abcd123ABCD";
    djv::replace_inplace(str2, L"ab", L"XX");
    EXPECT_EQ(str2, L"XXcd123XXCD");

    str2 = L"abcd123ABCD";
    djv::replace_inplace(str2, L"ab", L"XXXX");
    EXPECT_EQ(str2, L"XXXXcd123XXXXCD");

    str2 = L"abcd123ABCD";
    djv::replace_inplace(str2, L"123", L"***");
    EXPECT_EQ(str2, L"abcd***ABCD");

    str2 = L"abcd123ABCD";
    djv::replace_inplace(str2, L"123", L"*");
    EXPECT_EQ(str2, L"abcd*ABCD");
}

TEST(core_string, split_test)
{
    std::string str = "ab cd;12 34";
    auto parts = djv::split(str, "; ");
    EXPECT_EQ(parts.size(), 4);
    EXPECT_EQ(parts[0], "ab");
    EXPECT_EQ(parts[1], "cd");
    EXPECT_EQ(parts[2], "12");
    EXPECT_EQ(parts[3], "34");

    str = " ab ; cd ";
    parts = djv::split(str, "; ");
    EXPECT_EQ(parts.size(), 6);
    EXPECT_EQ(parts[0], "");
    EXPECT_EQ(parts[1], "ab");
    EXPECT_EQ(parts[2], "");
    EXPECT_EQ(parts[3], "");
    EXPECT_EQ(parts[4], "cd");
    EXPECT_EQ(parts[5], "");

    str = " ab ; cd ";
    parts = djv::split(str, "; ", true);
    EXPECT_EQ(parts.size(), 2);
    EXPECT_EQ(parts[0], "ab");
    EXPECT_EQ(parts[1], "cd");
}

/*
TEST(core_string, istring_split_test)
{
    using istring = std::basic_string<char, djv::ichar_traits<char>>;
    istring str = " 12 A 34 ";
    auto parts = djv::split(str, "a ", true);
    EXPECT_EQ(parts.size(), 2);
    EXPECT_EQ(parts[0], "12");
    EXPECT_EQ(parts[1], "34");
}
*/

TEST(core_string, split_wstring_test)
{
    std::wstring str = L"ab cd;12 34";
    auto parts = djv::split(str, L"; ");
    EXPECT_EQ(parts.size(), 4);
    EXPECT_EQ(parts[0], L"ab");
    EXPECT_EQ(parts[1], L"cd");
    EXPECT_EQ(parts[2], L"12");
    EXPECT_EQ(parts[3], L"34");

    str = L" ab ; cd ";
    parts = djv::split(str, L"; ");
    EXPECT_EQ(parts.size(), 6);
    EXPECT_EQ(parts[0], L"");
    EXPECT_EQ(parts[1], L"ab");
    EXPECT_EQ(parts[2], L"");
    EXPECT_EQ(parts[3], L"");
    EXPECT_EQ(parts[4], L"cd");
    EXPECT_EQ(parts[5], L"");

    str = L" ab ; cd ";
    parts = djv::split(str, L"; ", true);
    EXPECT_EQ(parts.size(), 2);
    EXPECT_EQ(parts[0], L"ab");
    EXPECT_EQ(parts[1], L"cd");
}

/*
TEST(core_string, istring_split_wstring_test)
{
    using iwstring = std::basic_string<wchar_t, djv::ichar_traits<wchar_t>>;
    iwstring str = L" 12 A 34 ";
    auto parts = djv::split(str, L"a ", true);
    EXPECT_EQ(parts.size(), 2);
    EXPECT_EQ(parts[0], L"12");
    EXPECT_EQ(parts[1], L"34");
}
*/