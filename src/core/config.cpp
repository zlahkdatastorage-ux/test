#include "pch.h"
#include "core/config.h"
#include "core/utils.h"

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
    menuToggleKey = VK_INSERT;
    showPerformance = true;
    compactMenu = false;
}

bool CoreConfig::save(const std::string& path) const
{
    FILE* fp = nullptr;
    if (fopen_s(&fp, path.c_str(), "w") != 0 || !fp)
        return false;

    fprintf(fp, "[logging]\n");
    fprintf(fp, "logToFile=%d\nlogToConsole=%d\nlogDebug=%d\nlogFile=%s\n",
            logToFile ? 1 : 0, logToConsole ? 1 : 0, logDebug ? 1 : 0, logFile.c_str());

    fprintf(fp, "[performance]\n");
    fprintf(fp, "frameRateTarget=%.3f\nuseVSync=%d\n",
            frameRateTarget, useVSync ? 1 : 0);

    fprintf(fp, "[scanner]\n");
    fprintf(fp, "autoScan=%d\nscanFrequencyHz=%u\n",
            autoScan ? 1 : 0, scanFrequencyHz);

    fprintf(fp, "[runtime]\n");
    fprintf(fp, "verboseInject=%d\nminimalHooks=%d\nreuseTimer=%d\nidleSleep=%d\n",
            verboseInject ? 1 : 0, minimalHooks ? 1 : 0,
            reuseTimer ? 1 : 0, idleSleep ? 1 : 0);

    fprintf(fp, "[ui]\n");
    fprintf(fp, "menuToggleKey=%d\nshowPerformance=%d\ncompactMenu=%d\n",
            menuToggleKey, showPerformance ? 1 : 0, compactMenu ? 1 : 0);

    fclose(fp);
    return true;
}

bool CoreConfig::load(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        return false;

    resetToDefaults();

    std::string section;
    std::string line;
    while (std::getline(file, line))
    {
        line = cheat::trim(line);
        if (line.empty() || line[0] == ';' || line[0] == '#')
            continue;

        if (line.front() == '[' && line.back() == ']')
        {
            section = cheat::toLower(line.substr(1, line.size() - 2));
            continue;
        }

        const size_t eq = line.find('=');
        if (eq == std::string::npos)
            continue;

        const std::string key = cheat::toLower(cheat::trim(line.substr(0, eq)));
        const std::string value = cheat::trim(line.substr(eq + 1));

        if (section == "logging")
        {
            if (key == "logtofile") logToFile = std::stoi(value) != 0;
            else if (key == "logtoconsole") logToConsole = std::stoi(value) != 0;
            else if (key == "logdebug") logDebug = std::stoi(value) != 0;
            else if (key == "logfile") logFile = value;
        }
        else if (section == "performance")
        {
            if (key == "frameratetarget") frameRateTarget = std::stof(value);
            else if (key == "usevsync") useVSync = std::stoi(value) != 0;
        }
        else if (section == "scanner")
        {
            if (key == "autoscan") autoScan = std::stoi(value) != 0;
            else if (key == "scanfrequencyhz") scanFrequencyHz = static_cast<u32>(std::stoul(value));
        }
        else if (section == "runtime")
        {
            if (key == "verboseinject") verboseInject = std::stoi(value) != 0;
            else if (key == "minimalhooks") minimalHooks = std::stoi(value) != 0;
            else if (key == "reusetimer") reuseTimer = std::stoi(value) != 0;
            else if (key == "idlesleep") idleSleep = std::stoi(value) != 0;
        }
        else if (section == "ui")
        {
            if (key == "menutogglekey") menuToggleKey = std::stoi(value);
            else if (key == "showperformance") showPerformance = std::stoi(value) != 0;
            else if (key == "compactmenu") compactMenu = std::stoi(value) != 0;
        }
    }

    return true;
}

} // namespace cheat
