#pragma once
#include "pch.h"
#include "core/config.h"
#include "core/logging.h"
#include "game/esp.h"
#include "game/aimbot.h"
#include "game/noclip.h"

namespace cheat
{

// ── Menu state ──────────────────────────────────────────────
enum class MenuState
{
    Hidden,
    Visible,
    Toggle
};

// ── Menu (console) ──────────────────────────────────────────
class Menu
{
public:
    Menu(HWND wnd = nullptr) : m_wnd(wnd) {}

    void show()
    {
        m_state = MenuState::Visible;
        CH_INFO("=== MyCheat Menu ===");
        CH_INFO("ESP : %s", g_ESP().enabled ? "ON" : "OFF");
        CH_INFO("Aimbot : %s", g_Aimbot().enabled ? "ON" : "OFF");
        CH_INFO("NoClip : %s", g_NoClip().enabled ? "ON" : "OFF");
    }

    void hide()
    {
        m_state = MenuState::Hidden;
    }

    MenuState state() const { return m_state; }
    void toggle() { m_state = (m_state == MenuState::Hidden) ? MenuState::Visible : MenuState::Hidden; }

private:
    HWND m_wnd = nullptr;
    MenuState m_state = MenuState::Hidden;
};

// ── Global menu instance ────────────────────────────────────
inline Menu& g_Menu()
{
    static Menu s;
    return s;
}

} // namespace cheat
