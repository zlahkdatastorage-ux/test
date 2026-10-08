#pragma once
#include "pch.h"
#include "core/config.h"
#include "core/logging.h"
#include "game/esp.h"
#include "game/aimbot.h"
#include "game/noclip.h"

namespace cheat
{

enum class MenuState
{
    Hidden,
    Visible
};

enum class MenuTab
{
    Overview,
    Features,
    Performance,
    Settings
};

class Menu
{
public:
    explicit Menu(HWND wnd = nullptr) : m_wnd(wnd) {}

    void show()
    {
        m_state = MenuState::Visible;
        print();
    }

    void hide()
    {
        m_state = MenuState::Hidden;
    }

    void toggle()
    {
        m_state = (m_state == MenuState::Hidden)
            ? MenuState::Visible
            : MenuState::Hidden;

        if (m_state == MenuState::Visible)
            print();
    }

    void nextTab()
    {
        const int next = (static_cast<int>(m_tab) + 1) % 4;
        m_tab = static_cast<MenuTab>(next);
        if (visible())
            print();
    }

    void previousTab()
    {
        const int previous = (static_cast<int>(m_tab) + 3) % 4;
        m_tab = static_cast<MenuTab>(previous);
        if (visible())
            print();
    }

    void selectTab(MenuTab tab)
    {
        m_tab = tab;
        if (visible())
            print();
    }

    void tick()
    {
        if (GetAsyncKeyState(g_Config().menuToggleKey) & 1)
            toggle();

        if (!visible())
            return;

        if (GetAsyncKeyState(VK_RIGHT) & 1)
            nextTab();
        else if (GetAsyncKeyState(VK_LEFT) & 1)
            previousTab();
    }

    MenuState state() const { return m_state; }
    MenuTab tab() const { return m_tab; }
    bool visible() const { return m_state == MenuState::Visible; }

    bool saveConfig(const std::string& path = "MyCheat.ini")
    {
        const bool ok = g_Config().save(path);
        if (ok)
            CH_INFO("Configuration saved: %s", path.c_str());
        else
            CH_ERROR("Failed to save configuration: %s", path.c_str());
        return ok;
    }

    bool loadConfig(const std::string& path = "MyCheat.ini")
    {
        const bool ok = g_Config().load(path);
        if (ok)
            CH_INFO("Configuration loaded: %s", path.c_str());
        else
            CH_WARN("Configuration not found: %s", path.c_str());
        return ok;
    }

private:
    void print() const
    {
        CH_INFO("========================================");
        CH_INFO(" MyCheat | %s", tabName());
        CH_INFO("========================================");

        switch (m_tab)
        {
        case MenuTab::Overview:
            CH_INFO("ESP       : %s", g_ESP().enabled ? "ON" : "OFF");
            CH_INFO("Aimbot    : %s", g_Aimbot().enabled ? "ON" : "OFF");
            CH_INFO("NoClip    : %s", g_NoClip().enabled ? "ON" : "OFF");
            CH_INFO("Menu key  : VK_%s", g_Config().menuToggleKey == VK_INSERT ? "INSERT" : "CUSTOM");
            break;

        case MenuTab::Features:
            CH_INFO("ESP       : %s", g_ESP().enabled ? "ON" : "OFF");
            CH_INFO("Aimbot    : %s", g_Aimbot().enabled ? "ON" : "OFF");
            CH_INFO("NoClip    : %s", g_NoClip().enabled ? "ON" : "OFF");
            CH_INFO("Use this tab as the feature registry grows.");
            break;

        case MenuTab::Performance:
            CH_INFO("Target FPS: %.1f", g_Config().frameRateTarget);
            CH_INFO("VSync     : %s", g_Config().useVSync ? "ON" : "OFF");
            CH_INFO("Perf panel: %s", g_Config().showPerformance ? "ON" : "OFF");
            CH_INFO("Compact UI : %s", g_Config().compactMenu ? "ON" : "OFF");
            break;

        case MenuTab::Settings:
            CH_INFO("Log file  : %s", g_Config().logFile.c_str());
            CH_INFO("Console   : %s", g_Config().logToConsole ? "ON" : "OFF");
            CH_INFO("File log  : %s", g_Config().logToFile ? "ON" : "OFF");
            CH_INFO("Debug log : %s", g_Config().logDebug ? "ON" : "OFF");
            CH_INFO("Config    : MyCheat.ini");
            break;
        }
    }

    const char* tabName() const
    {
        switch (m_tab)
        {
        case MenuTab::Overview:     return "Overview";
        case MenuTab::Features:     return "Features";
        case MenuTab::Performance:  return "Performance";
        case MenuTab::Settings:     return "Settings";
        default:                    return "Unknown";
        }
    }

    HWND m_wnd = nullptr;
    MenuState m_state = MenuState::Hidden;
    MenuTab m_tab = MenuTab::Overview;
};

inline Menu& g_Menu()
{
    static Menu s;
    return s;
}

} // namespace cheat
