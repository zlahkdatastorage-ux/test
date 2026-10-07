#pragma once
#include "pch.h"

#include <psapi.h>
#include <string>
#include "core/process.h"
#include "core/utils.h"

namespace cheat
{

// ── Game module detection (supports ros64.exe + mod DLLs) ──
struct GameModule
{
    std::string filename;    // e.g. "ros.exe", "game.dll"
    std::string path;
    uint64_t baseAddr = 0;
    uint64_t size = 0;
    bool    valid() const { return baseAddr != 0 && size > 0; }
};

// ── Find the game module in our own process (for injection) ─
inline std::vector<GameModule> findGameModules()
{
    std::vector<GameModule> mods;
    // Enumerate our own modules
    HMODULE hMods[1024];
    DWORD cb = sizeof(hMods) / sizeof(hMods[0]);
    if (EnumProcessModules(GetCurrentProcess(), hMods, sizeof(hMods), &cb))
    {
        for (DWORD i = 0; i < cb / sizeof(HMODULE); ++i)
        {
            CHAR szPath[MAX_PATH];
            if (GetModuleFileNameExA(GetCurrentProcess(), hMods[i], szPath, MAX_PATH))
            {
                GameModule gm;
                gm.path = szPath;
                gm.filename = strrchr(szPath, '\\') ? strrchr(szPath, '\\') + 1 : szPath;
                gm.baseAddr = (uint64_t)hMods[i];
                // approximate size
                MODULEINFO mi;
                if (GetModuleInformation(GetCurrentProcess(), hMods[i], &mi, sizeof(mi)))
                    gm.size = (uint64_t)mi.SizeOfImage;
                mods.push_back(gm);
            }
        }
    }
    return mods;
}

// ── Find a specific module by name ──────────────────────────
inline GameModule findModuleByName(const std::string& name)
{
    auto mods = findGameModules();
    for (auto& gm : mods)
    {
        if (toLower(gm.filename) == toLower(name))
            return gm;
    }
    return GameModule{};
}

// ── D3D device/device context manager (for renderer) ────────
struct D3DDevice
{
    ID3D11Device*            pDevice = nullptr;
    ID3D11DeviceContext*     pContext = nullptr;
    ID3D11RenderTargetView*  pRTV = nullptr;
    ID3D11DepthStencilView*  pDSV = nullptr;
    HWND                     hWnd = nullptr;
    bool                     initialized = false;
};

// ── Global D3D device (for ESP rendering) ───────────────────
inline D3DDevice& g_D3D()
{
    static D3DDevice s;
    return s;
}

} // namespace cheat
