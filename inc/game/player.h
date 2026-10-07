#pragma once
#include "pch.h"
#include "core/memory.h"
#include "core/cast.h"

namespace cheat
{

// ── Local player (our client) ───────────────────────────────
struct LocalPlayer
{
    bool valid = false;
    uint64_t client = 0;
    uint64_t position = 0;
    uint64_t health = 0;
    uint64_t velocity = 0;
    Vec3    worldPos;
    int     healthVal = 0;

    bool isAlive() const { return valid && healthVal > 0; }

    bool update(Memory& mem)
    {
        if (!valid || mem.read64(position, &position) == false) return false;
        // Note: real impl would read from the entity table
        mem.readFloat(position + 0x0C, &worldPos.x);  // approximate
        return true;
    }
};

// ── Global local player instance ────────────────────────────
inline LocalPlayer& g_Local()
{
    static LocalPlayer s;
    return s;
}

} // namespace cheat
