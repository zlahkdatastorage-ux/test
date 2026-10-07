#pragma once
#include "pch.h"

namespace cheat
{

// ── String helpers ──────────────────────────────────────────
inline std::string toLower(const std::string& s)
{
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return r;
}

inline std::string trim(const std::string& s)
{
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

inline std::string utf8ToWide(const std::string& s)
{
    if (s.empty()) return std::string();
    int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
    std::wstring w(n ? n : 1, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &w[0], n);
    return std::string(w.begin(), w.end());
}

inline std::string wideToUtf8(const std::wstring& w)
{
    if (w.empty()) return std::string();
    int n = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string s(n ? n : 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, &s[0], n, nullptr, nullptr);
    return s;
}

// ── Hexdump ─────────────────────────────────────────────────
inline void hexdump(const void* data, size_t len, int indent = 0)
{
    const unsigned char* p = (const unsigned char*)data;
    for (size_t i = 0; i < len; i += 16)
    {
        printf("%08zx: ", i);
        for (int j = 0; j < 16; ++j)
        {
            if (i + j < len)
                printf("%02x ", p[i + j]);
            else
                printf("   ");
        }
        printf(" ");
        for (int j = 0; j < 16 && i + j < len; ++j)
        {
            printf("%c", (p[i + j] >= 32 && p[i + j] < 127) ? p[i + j] : '.');
        }
        printf("\n");
    }
}

// ── Alignment ───────────────────────────────────────────────
inline size_t alignUp(size_t v, size_t align)
{
    return (v + align - 1) & ~(align - 1);
}
inline size_t alignDown(size_t v, size_t align)
{
    return v & ~(align - 1);
}

// ── Utility math ────────────────────────────────────────────
inline float clampf(float v, float lo, float hi)
{
    return v < lo ? lo : (v > hi ? hi : v);
}
inline int clampi(int v, int lo, int hi)
{
    return v < lo ? lo : (v > hi ? hi : v);
}

// ── Timing (QPC) ────────────────────────────────────────────
struct Timer
{
    static double frequency()
    {
        static bool init = false;
        static double freq = 0.0;
        if (!init)
        {
            LARGE_INTEGER fr;
            QueryPerformanceFrequency(&fr);
            freq = (double)fr.QuadPart;
            init = true;
        }
        return freq;
    }
    static double now()
    {
        LARGE_INTEGER t;
        QueryPerformanceCounter(&t);
        return (double)t.QuadPart / frequency();
    }
    static double elapsedSince(double t0) { return now() - t0; }
};

// ── DC brush/font cache (WMI-free, direct GDI) ──────────────
inline HDC GetDC(HWND wnd) { return wnd ? ::GetDC(wnd) : nullptr; }
inline void ReleaseDC(HWND wnd, HDC dc) { if (wnd && dc) ::ReleaseDC(wnd, dc); }

} // namespace cheat
