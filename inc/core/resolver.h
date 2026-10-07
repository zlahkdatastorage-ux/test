#pragma once
#include "pch.h"
#include "core/memory.h"

namespace cheat
{

// ── A single offset rule ────────────────────────────────────
struct OffsetRule
{
    enum class Source
    {
        Static,      // hardcoded base+offset
        Pointer,     // base + [offset1] + [offset2]...
        Pattern,     // byte pattern scan in the module
        Config       // loaded from a config file at runtime
    };

    Source   source = Source::Static;
    uint64_t baseAddr = 0;            // static base or module base
    std::string name;                 // human-readable name

    // For Pointer source
    std::vector<uint64_t>   chain;
    // For Pattern source
    std::vector<u8>         pattern;
    std::vector<char>       mask;   // 'x' = match, '?' = don't care

    // For Static source
    uint64_t offset = 0;

    // For Config source
    std::string configKey;

    bool isResolved() const
    {
        switch (source)
        {
        case Source::Static:      return baseAddr != 0 && offset != 0;
        case Source::Pointer:     return baseAddr != 0 && !chain.empty();
        case Source::Pattern:     return baseAddr != 0 && !pattern.empty() && !mask.empty();
        case Source::Config:      return !configKey.empty();
        default:                  return false;
        }
    }
};

// ── Offset resolver ─────────────────────────────────────────
class Resolver
{
public:
    Resolver() = default;

    /// Add a rule
    bool addRule(const OffsetRule& rule) { m_rules.push_back(rule); return true; }

    /// Resolve a named rule to a runtime address
    uint64_t resolve(const std::string& name) const
    {
        for (const auto& r : m_rules)
        {
            if (toLower(r.name) == toLower(name))
            {
                switch (r.source)
                {
                case OffsetRule::Source::Static:
                    if (r.baseAddr != 0 && r.offset != 0)
                        return r.baseAddr + r.offset;
                    break;
                case OffsetRule::Source::Pointer:
                    if (r.baseAddr != 0 && !r.chain.empty())
                    {
                        uint64_t cur = r.baseAddr;
                        for (auto off : r.chain)
                            cur = cur + off;
                        return cur;
                    }
                    break;
                case OffsetRule::Source::Pattern:
                    {
                        // Use Pattern matcher (implemented in pattern.h)
                        // Fallback: return 0 until scanner is integrated
                        return 0;
                    }
                case OffsetRule::Source::Config:
                    {
                        // Config loader will fill this at runtime
                        return 0;
                    }
                default: break;
                }
            }
        }
        return 0;
    }

    /// Get all rules
    std::vector<OffsetRule>& rules() { return m_rules; }

private:
    std::vector<OffsetRule> m_rules;
};

// ── Global resolver instance ────────────────────────────────
inline Resolver& g_Resolver()
{
    static Resolver s;
    return s;
}

} // namespace cheat
