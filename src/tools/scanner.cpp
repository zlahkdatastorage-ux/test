#include "pch.h"
#include "tools/scanner.h"
#include "core/module.h"
#include "core/logging.h"

namespace cheat
{

bool Scanner::scanModule(const std::string& moduleName)
{
    // Find the module in our own process (for injection)
    GameModule gm = findModuleByName(moduleName);
    if (!gm.valid())
    {
        CH_WARN("Scanner::scanModule - module '%s' not found", moduleName);
        return false;
    }
    CH_INFO("Found module: %s at 0x%llX size=0x%llX",
            gm.filename, gm.baseAddr, gm.size);
    return true;
}

ScanResult Scanner::findPattern(const Memory& mem, const std::string& module,
                                const std::vector<u8>& pattern,
                                const std::vector<char>& mask)
{
    ScanResult res;
    res.found = false;
    res.addr = 0;

    // Find module base and size
    GameModule gm = findModuleByName(module);
    if (!gm.valid())
    {
        CH_WARN("Scanner::findPattern - module '%s' not found", module);
        return res;
    }

    // Read the module memory
    std::vector<u8> data;
    if (!mem.readBytes(gm.baseAddr, data, gm.size))
    {
        CH_WARN("Scanner::findPattern - failed to read module memory");
        return res;
    }

    // Pattern scan
    uint64_t addr = PatternScanner::find(data.data(), data.size(), pattern, mask);
    if (addr != 0)
    {
        res.addr = addr - (uint64_t)data.data(); // offset relative to module base
        res.found = true;
        CH_INFO("Scanner::findPattern - pattern found at 0x%llX (offset 0x%llX)",
                res.addr, res.addr);
    }
    else
    {
        CH_WARN("Scanner::findPattern - pattern not found in '%s'", module);
    }
    return res;
}

uint64_t Scanner::findModuleBase(const Memory& mem, const std::string& moduleName)
{
    GameModule gm = findModuleByName(moduleName);
    if (gm.valid())
        return gm.baseAddr;
    return 0;
}

uint64_t Scanner::scan(const Memory& mem)
{
    // Scan for known patterns / module bases
    // In a real implementation this would iterate modules, find patterns,
    // and return the calculated address for a key offset.
    CH_INFO("Scanner::scan - scanning for patterns (stub)");
    return 0;
}

} // namespace cheat
