#pragma once
#include "pch.h"
#include "core/memory.h"
#include "core/cast.h"

namespace cheat
{

// ── Base entity ID (client-side handle) ─────────────────────
struct EntityID
{
    uint64_t handle = 0;
    bool valid() const { return handle != 0; }
    bool operator==(const EntityID& o) const { return handle == o.handle; }
};

// ── Client-side entity ▸ our game is likely a 3D title ──────
struct Entity
{
    EntityID id;
    uint64_t client = 0;
    uint64_t model = 0;
    uint64_t position = 0;   // Vec3 (x, y, z)
    uint64_t origin = 0;     // Vec3 (entity world position)
    uint64_t velocity = 0;
    uint64_t health = 0;
    uint64_t team = 0;
    uint64_t playerId = 0;   // e.g. "m_iPlayerSlot"
    uint64_t name = 0;       // name string
    uint64_t modelName = 0;  // model string

    // Computed
    Vec3 worldPos;
    std::string modelNameStr;
    std::string nameStr;
    int healthVal = 0;
    int teamVal = 0;

    bool isReady() const { return id.valid() && client != 0; }

    // Read from memory
    bool update(Memory& mem)
    {
        if (!isReady()) return false;
        // For a real implementation, this reads the entity table
        // Here we provide the struct with a read stub
        return true;
    }
};

// ═══════════════════════════════════════════════════════════
// Health / distance helpers
// ═══════════════════════════════════════════════════════════
inline int entityHealth(Entity& e) { return e.healthVal; }
inline float entityDistance(const Vec3& a, const Vec3& b)
{
    return vec3Distance(a, b);
}
inline float entityDistance(Entity& a, const Vec3& b)
{
    return vec3Distance(a.worldPos, b);
}

} // namespace cheat
