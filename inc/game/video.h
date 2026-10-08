#pragma once
#include "pch.h"
#include "core/module.h"

namespace cheat
{

// ── Display dimensions (updated every frame from the game) ──
struct Display
{
    int width = 0;
    int height = 0;
};

// ── Input state ─────────────────────────────────────────────
struct InputState
{
    bool forward = false;
    bool backward = false;
    bool left = false;
    bool right = false;
    bool jump = false;
    bool shoot = false;

    // Aimbot override
    bool aimOverride = false;
    cheat::Vec3 aimTarget;

    // Tick the input state
    void reset()
    {
        forward = backward = left = right = jump = shoot = false;
        aimOverride = false;
    }
};

// ── Global input (to mimic/control the game) ────────────────
inline InputState& g_Input()
{
    static InputState s;
    return s;
}

} // namespace cheat
