#include "pch.h"
#include "core/hook.h"

namespace cheat
{

bool HookManager::addHook(uint64_t addr, uint64_t detour, const std::string& name,
                          size_t len, HookType type)
{
    if (addr == 0 || detour == 0)
        return false;
    if (len > 15)
        len = 15; // max overwrite for x86/x64
    if (len == 0)
        return false;

    m_hooks.push_back({addr, detour, 0, len, type, false, name});
    return true;
}

bool HookManager::install(const ProcessInfo& pi, uint64_t gameBase)
{
    // In a full implementation this would:
    // 1. Allocate memory in the game process for the detour
    // 2. Write the hook bytes (E9 / FF 15 / etc.)
    // 3. Create a trampoline in our process or the game process
    // 4. Apply the hook
    // For now, we log that installation is a stub.
    std::string msg = "HookManager::install stub - hooks: ";
    msg += std::to_string(m_hooks.size());
    CH_INFO("%s", msg);
    return true;
}

bool HookManager::uninstall()
{
    // In a full implementation this would remove all hooks
    CH_INFO("HookManager::uninstall stub");
    return true;
}

bool HookManager::toggle(const std::string& name, bool enable)
{
    auto* h = findByName(name);
    if (!h) return false;
    h->enabled = enable;
    return true;
}

HookEntry* HookManager::findByName(const std::string& name)
{
    for (auto& h : m_hooks)
    {
        if (toLower(h.name) == toLower(name))
            return &h;
    }
    return nullptr;
}

} // namespace cheat
