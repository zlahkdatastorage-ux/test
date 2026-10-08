#pragma once
#include "pch.h"

namespace cheat
{

struct ProjectVersion
{
    u16 major = 0;
    u16 minor = 0;
    u16 patch = 0;
    u16 build = 0;

    std::string toString() const
    {
        char buf[64];
        snprintf(buf, sizeof(buf), "%u.%u.%u.%u", major, minor, patch, build);
        return std::string(buf);
    }
};

struct CoreConfig
{
    // Logging
    bool logToFile = true;
    bool logToConsole = true;
    bool logDebug = false;
    std::string logFile = "cheat.log";

    // Injection / hooking
    bool verboseInject = false;
    bool useSSRTng = false;
    bool minimalHooks = false;

    // Performance
    float frameRateTarget = 0.0f;
    bool useVSync = false;

    // Memory scanning
    bool autoScan = true;
    u32 scanFrequencyHz = 100;

    // Runtime behavior
    bool reuseTimer = true;
    bool idleSleep = true;

    // UI
    int menuToggleKey = VK_INSERT;
    bool showPerformance = true;
    bool compactMenu = false;

    void resetToDefaults();
    bool save(const std::string& path) const;
    bool load(const std::string& path);
};

inline CoreConfig& g_Config()
{
    static CoreConfig s;
    return s;
}

} // namespace cheat
