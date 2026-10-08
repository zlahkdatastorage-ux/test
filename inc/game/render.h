#pragma once
#include "pch.h"
#include "core/module.h"
#include "core/cast.h"
#include "game/video.h"
#include "core/diagnostics.h"

namespace cheat
{

// ── Draw primitive enumeration ──────────────────────────────
enum class DrawType
{
    Line,
    Box,
    Text,
    Sprite
};

// ── Renderer interface (DirectX 11) ─────────────────────────
class IRenderer
{
public:
    virtual ~IRenderer() = default;

    // Initialize (called after D3D device created)
    virtual bool init(HWND wnd) = 0;

    // Render a frame
    virtual void render() = 0;

    // Shutdown
    virtual void shutdown() = 0;

    // Draw a line
    virtual void drawLine(const Vec2& p1, const Vec2& p2, uint32_t color, float width = 1.0f) = 0;

    // Draw a filled rectangle
    virtual void drawRect(int x, int y, int w, int h, uint32_t color) = 0;

    // Draw text
    virtual void drawText(int x, int y, const std::string& text, uint32_t color,
                          int flags = 0) = 0;

    // Clean up internal resources
    virtual void cleanup() = 0;

    virtual bool initialized() const = 0;
    virtual FrameStats frameStats() const = 0;
};

// ── Renderer factory (returns a D3D11 renderer) ──────────────
std::unique_ptr<IRenderer> createRenderer();

// ── Coordinate transform: world point → screen point ────────
// In a real implementation this uses the view/projection matrix.
// For now we stub to return the input point.
inline Vec2 worldToScreen(const Vec3& world, int width, int height)
{
    // Stub: return a point near center
    return Vec2((float)width / 2.0f, (float)height / 2.0f);
}

// ── Global renderer instance ────────────────────────────────
// Note: This is defined in render.cpp, not here
std::unique_ptr<IRenderer>& g_Renderer();

} // namespace cheat
