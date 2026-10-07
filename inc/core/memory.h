#pragma once
#include "pch.h"
#include "core/process.h"

#include "core/utils.h"

namespace cheat
{

// ── Base memory reader/writer (tolerates invalid handles) ──
class Memory
{
public:
    Memory() = default;
    ~Memory() { close(); }

    bool open(const ProcessInfo& pi)
    {
        if (pi.hProcess == INVALID_HANDLE_VALUE) return false;
        m_h = pi.hProcess;
        return isValid();
    }

    void close()
    {
        if (m_h != nullptr && m_h != INVALID_HANDLE_VALUE)
        {
            CloseHandle(m_h);
            m_h = nullptr;
        }
    }

    bool isValid() const { return m_h != nullptr && m_h != INVALID_HANDLE_VALUE; }

    // ── Read primitives ─────────────────────────────────────
    bool read8(uint64_t addr, unsigned char* out) const
    {
        if (!isValid()) return false;
        SIZE_T bytes = 0;
        return ReadProcessMemory(m_h, (LPCVOID)addr, out, 1, &bytes) && bytes == 1;
    }
    bool read16(uint64_t addr, unsigned short* out) const
    {
        if (!isValid()) return false;
        SIZE_T bytes = 0;
        return ReadProcessMemory(m_h, (LPCVOID)addr, out, 2, &bytes) && bytes == 2;
    }
    bool read32(uint64_t addr, unsigned int* out) const
    {
        if (!isValid()) return false;
        SIZE_T bytes = 0;
        return ReadProcessMemory(m_h, (LPCVOID)addr, out, 4, &bytes) && bytes == 4;
    }
    bool read64(uint64_t addr, unsigned long long* out) const
    {
        if (!isValid()) return false;
        SIZE_T bytes = 0;
        return ReadProcessMemory(m_h, (LPCVOID)addr, out, 8, &bytes) && bytes == 8;
    }

    bool readFloat(uint64_t addr, float* out) const
    {
        if (!isValid()) return false;
        SIZE_T bytes = 0;
        return ReadProcessMemory(m_h, (LPCVOID)addr, out, 4, &bytes) && bytes == 4;
    }
    bool readDouble(uint64_t addr, double* out) const
    {
        if (!isValid()) return false;
        SIZE_T bytes = 0;
        return ReadProcessMemory(m_h, (LPCVOID)addr, out, 8, &bytes) && bytes == 8;
    }

    bool readStr(uint64_t addr, std::string& out, size_t maxLen = 256) const
    {
        if (!isValid()) return false;
        std::vector<char> buf(maxLen, '\0');
        SIZE_T bytes = 0;
        if (!ReadProcessMemory(m_h, (LPCVOID)addr, buf.data(), (SIZE_T)maxLen, &bytes))
            return false;
        // trim at first null
        for (SIZE_T i = 0; i < bytes; ++i)
            if (buf[i] == '\0') { bytes = i; break; }
        out.assign(buf.data(), (size_t)bytes);
        return !out.empty();
    }

    // ── Write primitives ────────────────────────────────────
    bool write8(uint64_t addr, unsigned char v) const
    {
        if (!isValid()) return false;
        return WriteProcessMemory(m_h, (LPVOID)addr, &v, 1, nullptr) != 0;
    }
    bool write16(uint64_t addr, unsigned short v) const
    {
        if (!isValid()) return false;
        return WriteProcessMemory(m_h, (LPVOID)addr, &v, 2, nullptr) != 0;
    }
    bool write32(uint64_t addr, unsigned int v) const
    {
        if (!isValid()) return false;
        return WriteProcessMemory(m_h, (LPVOID)addr, &v, 4, nullptr) != 0;
    }
    bool write64(uint64_t addr, unsigned long long v) const
    {
        if (!isValid()) return false;
        return WriteProcessMemory(m_h, (LPVOID)addr, &v, 8, nullptr) != 0;
    }
    bool writeFloat(uint64_t addr, float v) const
    {
        if (!isValid()) return false;
        return WriteProcessMemory(m_h, (LPVOID)addr, &v, 4, nullptr) != 0;
    }

    // ── Array read (with size) ──────────────────────────────
    bool readBytes(uint64_t addr, std::vector<unsigned char>& out, size_t count) const
    {
        if (!isValid() || count == 0) return false;
        out.resize(count);
        SIZE_T bytes = 0;
        if (!ReadProcessMemory(m_h, (LPCVOID)addr, out.empty() ? nullptr : &out[0], (SIZE_T)count, &bytes))
            return false;
        out.resize((size_t)bytes);
        return true;
    }

    bool readArray(uint64_t addr, std::vector<float>& out, size_t count) const
    {
        if (!isValid() || count == 0) return false;
        out.resize(count);
        SIZE_T bytes = 0;
        if (!ReadProcessMemory(m_h, (LPCVOID)addr, out.empty() ? nullptr : &out[0], (SIZE_T)(count * 4), &bytes))
            return false;
        out.resize((size_t)(bytes / 4));
        return true;
    }

    // ── Pointer chain (follow [base][off1][off2]...) ────────
    bool readChain(uint64_t base, const std::vector<uint64_t>& offsets,
                   uint64_t& out) const
    {
        uint64_t cur = base;
        for (size_t i = 0; i < offsets.size(); ++i)
        {
            if (!read64(cur + offsets[i], &cur))
                return false;
        }
        out = cur;
        return true;
    }

    uint64_t getBaseAddr() const { return m_baseAddr; }
    void setBaseAddr(uint64_t a) { m_baseAddr = a; }

private:
    HANDLE m_h = nullptr;
    uint64_t m_baseAddr = 0;
};

// ── Global memory handle (single process attached) ──────────
inline Memory& g_Mem()
{
    static Memory s;
    return s;
}

} // namespace cheat
