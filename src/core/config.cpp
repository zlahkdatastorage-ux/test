#include "pch.h"
#include "core/config.h"

namespace cheat
{
void CoreConfig::resetToDefaults()
{
    logToFile = true;
    logToConsole = true;
    logDebug = false;
    logFile = "cheat.log";
    verboseInject = false;
    useSSRTng = false;
    minimalHooks = false;
    frameRateTarget = 0.0f;
    useVSync = false;
    autoScan = true;
    scanFrequencyHz = 100;
    reuseTimer = true;
    idleSleep = true;
}

} // namespace cheat
