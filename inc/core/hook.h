#pragma once
#include "pch.h"

namespace cheat
{

// ── Hook types ──────────────────────────────────────────────
enum class HookType
{
    /// Direct relative call (x86: E8 xx xx xx xx ; x64: FF 15 / 48 B8 ...)
    Direct,
    /// JMP (E9 / E9 with 32-bit displacement)
    Jump,
    /// CALL (E8)
    Call,
    /// MOV to reg (e.g., B8 xx xx xx xx mov eax, imm32)
    Move,
    /// MS Detours style (Trampoline + DetourAttach via MinHook)
    MinHook,
    /// Microsoft Detours
    Detours
};

// ── Base hook descriptor ────────────────────────────────────
struct HookEntry
{
    uint64_t address      = 0;   // address to hook (in target process)
    uint64_t detour       = 0;   // address of our detour function (in this process)
    uint64_t original     = 0;   // saved original bytes (trampoline)
    size_t   len          = 0;   // number of bytes we overwrite (must be <= 15 on x86/x64)
    HookType type         = HookType::Direct;
    bool     enabled      = false;
    std::string name;

    /// Verify the hook can be installed at this address
    bool canInstall() const { return len > 0 && len <= 15; }
};

// ── Hook manager (injector context) ─────────────────────────
class HookManager
{
public:
    HookManager() = default;

    /// Register a hook to be installed
    bool addHook(uint64_t addr, uint64_t detour, const std::string& name,
                 size_t len = 5, HookType type = HookType::Direct);

    /// Install all registered hooks
    bool install(const ProcessInfo& pi, uint64_t gameBase);

    /// Uninstall all hooks
    bool uninstall();

    /// Toggle a hook on/off at runtime
    bool toggle(const std::string& name, bool enable);

    /// Find a hook by name
    HookEntry* findByName(const std::string& name);

    /// Count active hooks
    size_t count() const { return m_hooks.size(); }

private:
    std::vector<HookEntry> m_hooks;
};

// ── Global hook manager instance ────────────────────────────
inline HookManager& g_Hooks()
{
    static HookManager s;
    return s;
}

} // namespace cheat
