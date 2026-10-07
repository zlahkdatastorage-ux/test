#pragma once
#include "pch.h"

namespace cheat
{

// ── Project version ─────────────────────────────────────────
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

// ── Core configuration (runtime adjustable) ─────────────────
struct CoreConfig
{
    // Logging
    bool   logToFile       = true;
    bool   logToConsole    = true;
    bool   logDebug        = false;
    std::string logFile    = "cheat.log";

    // Injection / hooking
    bool   verboseInject   = false;
    bool   useSSRTng       = false;          // anti-anti-anti-cheat: SSR trampoline for hooks
    bool   minimalHooks    = false;          // only hook what's needed

    // Performance
    float  frameRateTarget = 0.0f;           // 0 = unlimited
    bool   useVSync        = false;

    // Memory scanning (for Ros64.exe / game module)
    bool   autoScan        = true;
    u32    scanFrequencyHz = 100;

    // Anti-detect / resilience
    bool   reuseTimer      = true;           // keep a single QPC timer for low overhead
    bool   idleSleep       = true;           // sleep when no frames happen

    void resetToDefaults();
};

inline CoreConfig& g_Config()
{
    static CoreConfig s;
    return s;
}

} // namespace cheat
