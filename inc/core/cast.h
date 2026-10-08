#pragma once
#include "pch.h"
#include "core/utils.h"

namespace cheat
{

// ── Safe bit reinterpretation via memcpy ─────────────────────
template <typename To, typename From>
inline To bit_cast(const From& v)
{
    static_assert(sizeof(To) == sizeof(From), "bit_cast: size mismatch");
    static_assert(std::is_trivially_copyable<To>::value,
                  "bit_cast destination must be trivially copyable");
    static_assert(std::is_trivially_copyable<From>::value,
                  "bit_cast source must be trivially copyable");

    To t{};
    memcpy(&t, &v, sizeof(To));
    return t;
}

template <typename To, typename From>
inline To asFloat(const From& v)
{
    return bit_cast<To>(v);
}

template <typename To, typename From>
inline To asInt(const From& v)
{
    return bit_cast<To>(v);
}

// ── Direct bit-level reading with defined edge cases ─────────
inline uint32_t readBits(uint32_t val, uint32_t shift, uint32_t bits)
{
    if (shift >= 32 || bits == 0)
        return 0;

    if (bits >= 32 - shift)
        return val >> shift;

    return (val >> shift) & ((uint32_t{1} << bits) - 1u);
}

// ── Byte swap (endianness) ──────────────────────────────────
inline uint32_t bswap32(uint32_t v)
{
    return _byteswap_ulong(v);
}

inline uint64_t bswap64(uint64_t v)
{
    return _byteswap_uint64(v);
}

// ── Bit manipulation helpers ────────────────────────────────
inline uint32_t popcount(uint32_t v)
{
    return __popcnt(v);
}

inline bool isPowerOfTwo(uint32_t v)
{
    return v && !(v & (v - 1));
}

// ── Color / ARGB packing ────────────────────────────────────
inline uint32_t packColor(u8 r, u8 g, u8 b, u8 a = 255)
{
    return (static_cast<uint32_t>(a) << 24) |
           (static_cast<uint32_t>(r) << 16) |
           (static_cast<uint32_t>(g) << 8) |
           static_cast<uint32_t>(b);
}

inline uint32_t packColor(float r, float g, float b, float a = 1.0f)
{
    const auto toByte = [](float v) -> u8
    {
        if (!std::isfinite(v))
            return 0;
        const float clamped = clampf(v, 0.0f, 1.0f);
        return static_cast<u8>(clamped * 255.0f + 0.5f);
    };

    return packColor(toByte(r), toByte(g), toByte(b), toByte(a));
}

inline uint32_t packColor(int r, int g, int b, int a = 255)
{
    return packColor(static_cast<u8>(clampi(r, 0, 255)),
                     static_cast<u8>(clampi(g, 0, 255)),
                     static_cast<u8>(clampi(b, 0, 255)),
                     static_cast<u8>(clampi(a, 0, 255)));
}

// ── Vector math ──────────────────────────────────────────────
struct Vec2
{
    float x, y;
    Vec2() : x(0), y(0) {}
    Vec2(float _x, float _y) : x(_x), y(_y) {}
    bool operator==(const Vec2& o) const { return x == o.x && y == o.y; }
};

struct Vec3
{
    float x, y, z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    bool operator==(const Vec3& o) const { return x == o.x && y == o.y && z == o.z; }
};

struct Vec4
{
    float x, y, z, w;
    Vec4() : x(0), y(0), z(0), w(0) {}
    Vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
};

inline float vec2Length(const Vec2& v)
{
    return std::sqrt(v.x * v.x + v.y * v.y);
}

inline float vec3Length(const Vec3& v)
{
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline float vec2Distance(const Vec2& a, const Vec2& b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

inline float vec3Distance(const Vec3& a, const Vec3& b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

inline Vec3 vec3Normalize(const Vec3& v)
{
    float l = vec3Length(v);
    if (l < 1e-9f) return Vec3(0, 0, 0);
    return Vec3(v.x / l, v.y / l, v.z / l);
}

inline Vec3 vec3Add(const Vec3& a, const Vec3& b)
{
    return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vec3 vec3Sub(const Vec3& a, const Vec3& b)
{
    return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vec3 vec3Scale(const Vec3& v, float s)
{
    return Vec3(v.x * s, v.y * s, v.z * s);
}

// ── 4x4 matrix helpers ──────────────────────────────────────
struct Mat4
{
    float m[4][4];
    Mat4() { memset(m, 0, sizeof(m)); }
    float* operator[](int r) { return m[r]; }
    const float* operator[](int r) const { return m[r]; }
    float* operator()(int r, int c) { return &m[r][c]; }
    const float* operator()(int r, int c) const { return &m[r][c]; }
};

inline float degToRad(float deg) { return deg * 3.14159265358979323846f / 180.0f; }
inline float radToDeg(float rad) { return rad * 180.0f / 3.14159265358979323846f; }

} // namespace cheat
