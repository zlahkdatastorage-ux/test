#pragma once
#include "pch.h"
#include "tools/scanner.h"

namespace cheat
{

// ── Pattern scanner implementation ──────────────────────────
// We'll use a fast pattern scanning utility
class PatternScanner
{
public:
    // Classic pattern scan: pattern is like "xx????xxxx" where x = byte, ? = wildcard
    static uint64_t find(const u8* data, size_t dataLen,
                         const std::vector<u8>& pat,
                         const std::vector<char>& mask)
    {
        if (pat.size() != mask.size() || dataLen < pat.size())
            return 0;

        for (size_t i = 0; i + pat.size() <= dataLen; ++i)
        {
            bool match = true;
            for (size_t j = 0; j < pat.size(); ++j)
            {
                if (mask[j] == 'x' && data[i + j] != pat[j])
                {
                    match = false;
                    break;
                }
            }
            if (match)
                return (uint64_t)(data + i);
        }
        return 0;
    }

    // Find all matches (with distance limit)
    static std::vector<uint64_t> findAll(const u8* data, size_t dataLen,
                                         const std::vector<u8>& pat,
                                         const std::vector<char>& mask,
                                         size_t maxResults = 16)
    {
        std::vector<uint64_t> results;
        if (pat.size() != mask.size() || dataLen < pat.size())
            return results;

        for (size_t i = 0; i + pat.size() <= dataLen; ++i)
        {
            bool match = true;
            for (size_t j = 0; j < pat.size(); ++j)
            {
                if (mask[j] == 'x' && data[i + j] != pat[j])
                {
                    match = false;
                    break;
                }
            }
            if (match)
            {
                results.push_back((uint64_t)(data + i));
                if (results.size() >= maxResults)
                    break;
            }
        }
        return results;
    }
};

} // namespace cheat
