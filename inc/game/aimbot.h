#pragma once
#include "pch.h"
#include "core/memory.h"
#include "game/entity.h"
#include "game/player.h"

namespace cheat
{

// ── Aimbot settings ─────────────────────────────────────────
struct AimbotConfig
{
    bool enabled = false;
    bool fovMode = true;      // FOV-based targeting
    bool smartAim = false;    // prioritize closest / healthiest
    bool rapidFire = false;   // auto-fire toggle
    bool autoScope = false;   // scope toggling
    int    fov = 10;          // FOV threshold in degrees
    float  maxDist = 500.0f;  // max target distance
    float  heightOffset = 20.0f; // vertical aim offset
    float  creepDamage = 1.0f;   // ratio of health to aim at

    uint32_t crosshairColor = packColor(255, 255, 255, 255);
    uint32_t tracColor = packColor(255, 0, 0, 180);

    void reset()
    {
        enabled = false;
        fovMode = true;
        smartAim = false;
        rapidFire = false;
        autoScope = false;
        fov = 10;
        maxDist = 500.0f;
        heightOffset = 20.0f;
        creepDamage = 1.0f;
        crosshairColor = packColor(255, 255, 255, 255);
        tracColor = packColor(255, 0, 0, 180);
    }
};

inline AimbotConfig& g_Aimbot()
{
    static AimbotConfig s;
    return s;
}

// ── Aimbot targeting ────────────────────────────────────────
// Returns the best target from a list based on config.
inline Entity* findBestTarget(const std::vector<Entity>& targets)
{
    if (!g_Aimbot().enabled || targets.empty()) return nullptr;

    Entity* best = nullptr;
    float bestScore = 0.0f;

    for (auto& e : targets)
    {
        if (!e.isReady()) continue;

        // Distance check
        if (e.worldPos.z > g_Aimbot().maxDist) continue;

        // FOV check (heuristic)
        float dist = vec3Distance(g_Local().worldPos, e.worldPos);
        float score = 1.0f / (dist + 1.0f);

        // Health weighting
        float healthRatio = e.healthVal > 0 ? (float)e.healthVal / 100.0f : 1.0f;
        score *= healthRatio;

        // Smart aim: prefer higher score
        if (!best || score > bestScore)
        {
            best = &e;
            bestScore = score;
        }
    }
    return best;
}

// ── Rapid fire helper ───────────────────────────────────────
inline void rapidFire(uint64_t shootAddr)
{
    // In a real implementation this writes timing + shoot function calls
    // For now, a stub.
}

} // namespace cheat
