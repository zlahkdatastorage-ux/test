#pragma once
#include "pch.h"
#include "core/memory.h"
#include "game/entity.h"

namespace cheat
{

// ── Game mode classification (for feature fitting) ──────────
enum class GameMode
{
    Unhandled,   // Unknown
    Rifle,       // FPS with rifles
    Shooter,     // Multiplayer shooter
    BattleRoyale // Battle royale format (e.g. BR matches)
};

// ── Helper to detect game mode via entity + config ──────────
inline GameMode detectGameMode(Memory& mem)
{
    // This is a heuristic; real detection needs a config + pattern scan
    return GameMode::Unhandled;
}

// ── Game rules (time, round, match state) ───────────────────
struct GameRules
{
    bool inGame = false;
    bool matchStarted = false;
    float time = 0.0f;
    int round = 0;
    bool paused = false;
};

} // namespace cheat
