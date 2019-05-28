#pragma once

namespace GSZHU {
    class CNonCopyable {
    public:
        CNonCopyable(CNonCopyable const&) = delete;
        CNonCopyable(CNonCopyable&&) = delete;
        CNonCopyable& operator=(CNonCopyable const&) = delete;
        CNonCopyable& operator=(CNonCopyable&&) = delete;
    };
}