#pragma once

#include "ILogger.h"

namespace Wanlix
{
    class ConsoleLogger : public ILogger
    {
    public:
        ConsoleLogger() noexcept;
        virtual ~ConsoleLogger() = default;

        virtual void Log(LogType type, StringCRef msg) const override;

    private:
        void LogCyan(const char* msg) const;
        void LogYellow(const char* msg) const;
        void LogRed(const char* msg) const;
        void LogNoColor(const char* msg) const;
    };
}