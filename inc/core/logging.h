#pragma once
#include "pch.h"

namespace cheat
{

enum class LogLevel
{
    None      = 0,
    Error     = 1,
    Warning   = 2,
    Info      = 3,
    Debug     = 4,
    Trace     = 5
};

class Logger
{
public:
    static void initialize(const std::string& filename, bool toConsole, bool toFile, LogLevel level);

    static void log(LogLevel level, const std::string& file, int line, const char* fmt, ...);

    template <typename... Args>
    static void logLine(LogLevel level, const std::string& message)
    {
        static std::mutex mtx;
        std::lock_guard<std::mutex> lock(mtx);
        if (level > s_Level || (level == LogLevel::Debug && !s_Debug))
            return;

        SYSTEMTIME st;
        GetLocalTime(&st);
        char ts[32];
        snprintf(ts, sizeof(ts), "%02d:%02d:%02d.%03d",
                 st.wHour, st.wMinute, st.wSecond,
                 static_cast<int>(GetTickCount() % 1000));

        const char* color = "\x1b[0m";
        switch (level)
        {
        case LogLevel::Error:   color = "\x1b[31m"; break;
        case LogLevel::Warning: color = "\x1b[33m"; break;
        case LogLevel::Info:    color = "\x1b[32m"; break;
        case LogLevel::Debug:   color = "\x1b[36m"; break;
        case LogLevel::Trace:   color = "\x1b[35m"; break;
        default: break;
        }

        if (s_LogToConsole)
            printf("[%s] %sLOG\x1b[0m | %s\n", ts, color, message.c_str());

        if (s_LogToFile)
        {
            FILE* fp = nullptr;
            if (fopen_s(&fp, s_LogFile.c_str(), "a") == 0 && fp)
            {
                fprintf(fp, "[%s] LOG | %s\n", ts, message.c_str());
                fclose(fp);
            }
        }
    }

    static void setLevel(LogLevel level) { s_Level = level; }
    static LogLevel level() { return s_Level; }
    static void setDebug(bool d) { s_Debug = d; }
    static bool debug() { return s_Debug; }
    static void setLogFile(const std::string& f) { s_LogFile = f; }
    static std::string logFile() { return s_LogFile; }
    static void setLogToFile(bool f) { s_LogToFile = f; }
    static bool logToFile() { return s_LogToFile; }
    static void setLogToConsole(bool f) { s_LogToConsole = f; }
    static bool logToConsole() { return s_LogToConsole; }

    template <typename... Args>
    static void error(const std::string& fmt, Args... args)
    {
        log(LogLevel::Error, __FILE__, __LINE__,
            fmt.c_str(), args...);
    }

    template <typename... Args>
    static void warn(const std::string& fmt, Args... args)
    {
        log(LogLevel::Warning, __FILE__, __LINE__,
            fmt.c_str(), args...);
    }

    template <typename... Args>
    static void info(const std::string& fmt, Args... args)
    {
        log(LogLevel::Info, __FILE__, __LINE__,
            (fmt + "\n").c_str(), args...);
    }

    template <typename... Args>
    static void debug(const std::string& fmt, Args... args)
    {
        if (s_Debug)
            log(LogLevel::Debug, __FILE__, __LINE__,
                fmt + "\n", args...);
    }

private:
    static LogLevel   s_Level;
    static bool       s_Debug;
    static std::string s_LogFile;
    static bool       s_LogToFile;
    static bool       s_LogToConsole;
};

// Helper macros matching common cheat conventions
#define CH_LOG(level, ...) cheat::Logger::logLine(level, std::string(__VA_ARGS__))
#define CH_ERROR(...) cheat::Logger::error(__VA_ARGS__)
#define CH_WARN(...)  cheat::Logger::warn(__VA_ARGS__)
#define CH_INFO(...)  cheat::Logger::info(__VA_ARGS__)
#define CH_DEBUG(...) cheat::Logger::debug(__VA_ARGS__)
#define CH_TRACE(...) cheat::Logger::logLine(LogLevel::Trace, __VA_ARGS__)

} // namespace cheat
