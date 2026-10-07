#pragma once
#include "pch.h"
#include "core/memory.h"
#include "game/player.h"

namespace cheat
{

// ── No clip settings ────────────────────────────────────────
struct NoClipConfig
{
    bool enabled = false;
    float speed = 2.0f;       // movement speed
    bool crouch = false;
    bool jump = false;

    void reset()
    {
        enabled = false;
        speed = 2.0f;
        crouch = false;
        jump = false;
    }
};

inline NoClipConfig& g_NoClip()
{
    static NoClipConfig s;
    return s;
}

// ── No clip logic (applied to local player position) ────────
inline void updateNoClip(Memory& mem, LocalPlayer& lp)
{
    if (!g_NoClip().enabled || !lp.valid) return;

    // In a real implementation this would write to the player position
    // and bypass collision/grounding checks. Stubbed.
}

} // namespace cheat
