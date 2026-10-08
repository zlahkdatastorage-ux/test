#include "pch.h"
#include "core/logging.h"

namespace cheat
{

LogLevel   Logger::s_Level   = LogLevel::Info;
bool       Logger::s_Debug   = false;
std::string Logger::s_LogFile = "cheat.log";
bool       Logger::s_LogToFile = true;
bool       Logger::s_LogToConsole = true;

void Logger::initialize(const std::string& filename, bool toConsole, bool toFile, LogLevel level)
{
    s_LogFile = filename;
    s_LogToConsole = toConsole;
    s_LogToFile = toFile;
    s_Level = level;
    // Clear existing log file
    if (toFile)
    {
        FILE* fp = fopen(filename.c_str(), "w");
        if (fp) fclose(fp);
    }
}

void Logger::log(LogLevel level, const std::string& file, int line,
                 const char* fmt, ...)
{
    if (level > s_Level)
        return;

    // Timestamp
    static const char* spaces = "                                                    ";
    SYSTEMTIME st;
    GetLocalTime(&st);
    char ts[32];
    snprintf(ts, sizeof(ts), "%02d:%02d:%02d.%03d | ",
             st.wHour, st.wMinute, st.wSecond, (int)(GetTickCount() % 1000));

    // Format message
    va_list args;
    va_start(args, fmt);
    char buf[4096];
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    const char* color = "\x1b[0m";
    switch (level)
    {
    case LogLevel::Error:   color = "\x1b[31m"; break;
    case LogLevel::Warning: color = "\x1b[33m"; break;
    case LogLevel::Info:    color = "\x1b[32m"; break;
    case LogLevel::Debug:   color = "\x1b[36m"; break;
    case LogLevel::Trace:   color = "\x1b[35m"; break;
    default:                color = "\x1b[0m"; break;
    }

    std::string fileShort = file.substr(file.find_last_of("\\/") + 1);
    std::string msg = std::string(buf);

    if (s_LogToConsole)
    {
        printf("[%s%u] %s%-5s%s | %s%s%s\n",
               ts, line, color, fileShort, "\x1b[0m",
               msg.c_str(), color, "", "");
    }

    if (s_LogToFile)
    {
        static std::mutex mtx;
        std::lock_guard<std::mutex> lock(mtx);
        FILE* fp = fopen(s_LogFile.c_str(), "a");
        if (fp)
        {
            fprintf(fp, "[%s%u] %s%-5s%s | %s\n",
                    ts, line, color, fileShort, "\x1b[0m",
                    msg.c_str());
            fclose(fp);
        }
    }
}

} // namespace cheat
