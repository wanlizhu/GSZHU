#pragma once

#include "Wanlix/Core/EngineConfig.h"
#include "ILogger.h"

namespace Wanlix
{
    class ConsoleLogger : public ILogger
    {
    public:
        ConsoleLogger();
        virtual ~ConsoleLogger() {}

        virtual void Log(LogType type, const std::string& msg) const override;

    private:
        void LogCyan(const char* msg) const;
        void LogYellow(const char* msg) const;
        void LogRed(const char* msg) const;
        void LogNoColor(const char* msg) const;
    };
}