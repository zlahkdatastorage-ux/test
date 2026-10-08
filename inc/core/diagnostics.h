#pragma once
#include "pch.h"

namespace cheat
{

struct FrameStats
{
    double frameMs = 0.0;
    double averageMs = 0.0;
    double minMs = 0.0;
    double maxMs = 0.0;
    uint64_t frames = 0;
};

class FrameProfiler
{
public:
    void begin()
    {
        m_start = Clock::now();
    }

    void end()
    {
        const double ms =
            std::chrono::duration<double, std::milli>(Clock::now() - m_start).count();

        m_current.frameMs = ms;
        ++m_current.frames;

        if (m_current.frames == 1)
        {
            m_current.averageMs = ms;
            m_current.minMs = ms;
            m_current.maxMs = ms;
            return;
        }

        m_current.averageMs +=
            (ms - m_current.averageMs) / static_cast<double>(m_current.frames);

        m_current.minMs = (std::min)(m_current.minMs, ms);
        m_current.maxMs = (std::max)(m_current.maxMs, ms);
    }

    const FrameStats& stats() const { return m_current; }

    void reset()
    {
        m_current = {};
        m_start = Clock::now();
    }

private:
    using Clock = std::chrono::steady_clock;

    Clock::time_point m_start = Clock::now();
    FrameStats m_current{};
};

inline FrameProfiler& g_FrameProfiler()
{
    static FrameProfiler profiler;
    return profiler;
}

class ScopedFrameProfile
{
public:
    ScopedFrameProfile() { g_FrameProfiler().begin(); }
    ~ScopedFrameProfile() { g_FrameProfiler().end(); }

    ScopedFrameProfile(const ScopedFrameProfile&) = delete;
    ScopedFrameProfile& operator=(const ScopedFrameProfile&) = delete;
};

} // namespace cheat
