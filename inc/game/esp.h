#pragma once
#include "pch.h"
#include "core/memory.h"
#include "game/entity.h"

namespace cheat
{

// ── ESP (edge surface) settings ─────────────────────────────
struct ESPConfig
{
    bool enabled = false;
    bool box = true;        // entity box
    bool health = true;     // health text
    bool name = true;       // name above entity
    bool distance = true;   // distance text
    bool team = false;      // team color
    bool chams = false;     // wireframe chams
    bool glow = false;      // glow outline

    // Colors (ARGB)
    uint32_t boxColor = packColor(0, 255, 0, 200);
    uint32_t boxOutline = packColor(0, 255, 0, 255);
    uint32_t healthColor = packColor(255, 0, 0, 255);
    uint32_t nameColor = packColor(0, 255, 255, 255);

    // Distance threshold
    float maxDistance = 500.0f;

    // Update
    void reset()
    {
        enabled = false;
        box = true;
        health = true;
        name = true;
        distance = true;
        team = false;
        chams = false;
        glow = false;
        boxColor = packColor(0, 255, 0, 200);
        boxOutline = packColor(0, 255, 0, 255);
        healthColor = packColor(255, 0, 0, 255);
        nameColor = packColor(0, 255, 255, 255);
        maxDistance = 500.0f;
    }
};

inline ESPConfig& g_ESP()
{
    static ESPConfig s;
    return s;
}

// ── ESP rendering ───────────────────────────────────────────
inline void renderESP(Memory& mem, const std::vector<Entity>& entities)
{
    if (!g_ESP().enabled || entities.empty()) return;

    // In a full implementation this iterates the entity list
    // and draws the box/health/name using the renderer.
    // For now we provide the hook point.
}

// ── ESP toggle helper ───────────────────────────────────────
inline void toggleESP(bool on)
{
    g_ESP().enabled = on;
}

// ── Get screen position of an entity (stub) ─────────────────
inline Vec2 getScreenPosition(const Entity& e, int w, int h)
{
    // Stub: center
    return Vec2((float)w / 2.0f, (float)h / 2.0f);
}

} // namespace cheat
