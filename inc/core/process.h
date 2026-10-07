#pragma once
#include "pch.h"

#include "core/utils.h"

namespace cheat
{

// ── Process information ─────────────────────────────────────
struct ProcessInfo
{
    HANDLE  hProcess = INVALID_HANDLE_VALUE; // INVALID_HANDLE_VALUE = not open
    DWORD   pid      = 0;
    std::string name;
    std::string path;
    uint64_t baseAddr = 0; // base address of loaded module (for main module)
    bool    valid() const { return hProcess != INVALID_HANDLE_VALUE && pid != 0; }
};

// ── Enumerate all processes ─────────────────────────────────
inline std::vector<ProcessInfo> enumerateProcesses()
{
    std::vector<ProcessInfo> list;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) return list;

    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(pe);
    if (Process32FirstW(hSnap, &pe))
    {
        do
        {
            ProcessInfo pi;
            pi.pid = pe.th32ProcessID;
            // get full name
            std::wstring name(pe.szExeFile);
            pi.name = wideToUtf8(name);
            if (pi.name.empty() || pi.name == "?")
                pi.name = std::to_string(pi.pid);

            // get full path from module snapshot
            HANDLE hModSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pi.pid);
            if (hModSnap != INVALID_HANDLE_VALUE)
            {
                MODULEENTRY32W me;
                me.dwSize = sizeof(me);
                if (Module32FirstW(hModSnap, &me))
                {
                    std::wstring path(me.szExePath);
                    pi.path = wideToUtf8(path);
                }
                CloseHandle(hModSnap);
            }
            list.push_back(pi);
        } while (Process32NextW(hSnap, &pe));
    }
    CloseHandle(hSnap);
    return list;
}

// ── Open a process with full access (for read/write) ───────
inline ProcessInfo openProcess(const ProcessInfo& info)
{
    ProcessInfo pi;
    pi.pid = info.pid;
    pi.path = info.path;
    pi.name = info.name;
    if (pi.pid == 0) return pi;

    pi.hProcess = OpenProcess(
        PROCESS_QUERY_LIMITED_INFORMATION |
        PROCESS_VM_READ |
        PROCESS_VM_WRITE |
        PROCESS_VM_OPERATION |
        PROCESS_CREATE_THREAD |
        PROCESS_QUERY_INFORMATION,
        FALSE, pi.pid);

    if (pi.hProcess)
    {
        // resolve the main module base address
        HANDLE hModSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pi.pid);
        if (hModSnap != INVALID_HANDLE_VALUE)
        {
            MODULEENTRY32W me;
            me.dwSize = sizeof(me);
            if (Module32FirstW(hModSnap, &me))
            {
                std::wstring path(me.szExePath);
                pi.path = wideToUtf8(path);
                pi.baseAddr = (uint64_t)me.modBaseAddr;
            }
            CloseHandle(hModSnap);
        }
    }
    return pi;
}

// ── Find a process by name (case-insensitive) ──────────────
inline ProcessInfo openProcessByName(const std::string& name)
{
    ProcessInfo found;
    auto procs = enumerateProcesses();
    for (auto& pi : procs)
    {
        if (toLower(pi.name) == toLower(name))
        {
            found = openProcess(pi);
            if (found.valid())
                return found;
        }
    }
    // fall back: exact pid? Try searching by partial match
    for (auto& pi : procs)
    {
        if (toLower(pi.name).find(toLower(name)) != std::string::npos)
        {
            found = openProcess(pi);
            if (found.valid())
                return found;
        }
    }
    return found;
}
    ProcessInfo found;
    auto procs = enumerateProcesses();
    for (auto& pi : procs)
    {
        if (toLower(pi.name) == toLower(name))
        {
            found = openProcess(pi);
            if (found.valid())
                return found;
        }
    }
    // fall back: exact pid? Try searching by partial match
    for (auto& pi : procs)
    {
        if (toLower(pi.name).find(toLower(name)) != std::string::npos)
        {
            found = openProcess(pi);
            if (found.valid())
                return found;
        }
    }
    return found;
}

// ── Open a process with full access (for read/write) ───────
inline ProcessInfo openProcess(const ProcessInfo& info)
{
    ProcessInfo pi;
    pi.pid = info.pid;
    pi.path = info.path;
    pi.name = info.name;
    if (pi.pid == 0) return pi;

    pi.hProcess = OpenProcess(
        PROCESS_QUERY_LIMITED_INFORMATION |
        PROCESS_VM_READ |
        PROCESS_VM_WRITE |
        PROCESS_VM_OPERATION |
        PROCESS_CREATE_THREAD |
        PROCESS_QUERY_INFORMATION,
        FALSE, pi.pid);

    if (pi.hProcess)
    {
        // resolve the main module base address
        HANDLE hModSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pi.pid);
        if (hModSnap != INVALID_HANDLE_VALUE)
        {
            MODULEENTRY32W me;
            me.dwSize = sizeof(me);
            if (Module32FirstW(hModSnap, &me))
            {
                std::wstring path(me.szExePath);
                pi.path = wideToUtf8(path);
                pi.baseAddr = (uint64_t)me.modBaseAddr;
            }
            CloseHandle(hModSnap);
        }
    }
    return pi;
}

} // namespace cheat
