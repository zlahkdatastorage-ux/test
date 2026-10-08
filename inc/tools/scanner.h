#pragma once
#include "pch.h"
#include "core/memory.h"

namespace cheat
{

// ── Base type for byte scanning ─────────────────────────────
// u8 is defined in pch.h; here for clarity
using Byte = u8;

// ── Scan result ─────────────────────────────────────────────
struct ScanResult
{
    uint64_t addr = 0;
    uint32_t offset = 0;
    bool     found = false;
};

// ── Scanner engine for Ros64.exe / game modules ─────────────
class Scanner
{
public:
    Scanner() = default;

    // Find a module by name (scans our process; we'll later support targeting a game process)
    bool scanModule(const std::string& moduleName);

    // Pattern scan a memory region
    template <typename... Args>
    uint64_t patternScan(uint64_t base, uint64_t size, const std::vector<Byte>& pattern,
                         const std::vector<char>& mask)
    {
        Byte* data = nullptr;
        // Read process memory into a buffer
        // In a full implementation, we'd handle this via Memory::readBytes
        // For now, stub returns 0
        return 0;
    }

    // Find a pattern within a module
    ScanResult findPattern(const Memory& mem, const std::string& module,
                           const std::vector<Byte>& pattern, const std::vector<char>& mask);

    // Quick scan for a module base address
    uint64_t findModuleBase(const Memory& mem, const std::string& moduleName);

    // Scan the game for a known address (from the config)
    uint64_t scan(const Memory& mem);

private:
    std::vector<Byte> m_buffer;
    size_t m_bufferSize = 0;
};

// ── Global scanner instance ────────────────────────────────
inline Scanner& g_Scanner()
{
    static Scanner s;
    return s;
}

} // namespace cheat
