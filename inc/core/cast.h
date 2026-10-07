#pragma once
#include "pch.h"

namespace cheat
{

// ── Safe reinterpret via memcpy (avoid UB warnings) ─────────
template <typename T, typename U>
inline T bit_cast(U&& v)
{
    static_assert(sizeof(T) == sizeof(U), "bit_cast: size mismatch");
    T t;
    memcpy(&t, &v, sizeof(T));
    return t;
}

template <typename T>
inline T asFloat(T v)
{
    return bit_cast<T>(v);
}
template <typename T>
inline T asInt(T v)
{
    return bit_cast<T>(v);
}

// ── Direct bit-level reading with masking / shifting ────────
inline uint32_t readBits(uint32_t val, uint32_t shift, uint32_t bits)
{
    return (val >> shift) & ((1u << bits) - 1);
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
    return (a << 24) | (r << 16) | (g << 8) | b;
}
inline uint32_t packColor(float r, float g, float b, float a = 1.0f)
{
    return packColor((u8)(r * 255.0f), (u8)(g * 255.0f), (u8)(b * 255.0f), (u8)(a * 255.0f));
}
inline uint32_t packColor(int r, int g, int b, int a = 255)
{
    return packColor((u8)clampi(r, 0, 255), (u8)clampi(g, 0, 255), (u8)clampi(b, 0, 255), (u8)clampi(a, 0, 255));
}

// ── Vector math (single precision, inline) ──────────────────
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

// ── 3x3 / 4x4 matrix helpers (for view/projection) ─────────
struct Mat4
{
    float m[4][4];
    Mat4() { memset(m, 0, sizeof(m)); }
    float* operator[](int r) { return m[r]; }
    const float* operator[](int r) const { return m[r]; }
    float* operator()(int r, int c) { return &m[r][c]; }
    const float* operator()(int r, int c) const { return &m[r][c]; }
};

// ── Camera frustum / projection helpers ─────────────────────
inline float degToRad(float deg) { return deg * 3.14159265358979323846f / 180.0f; }
inline float radToDeg(float rad) { return rad * 180.0f / 3.14159265358979323846f; }

} // namespace cheat
