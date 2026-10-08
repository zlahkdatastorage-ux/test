#include "pch.h"
#include "game/render.h"
#include "core/logging.h"

namespace cheat
{

// ── D3D11 renderer implementation ───────────────────────────
class D3D11Renderer : public IRenderer
{
public:
    bool init(HWND wnd) override
    {
        m_hWnd = wnd;
        if (!wnd) return false;

        // Create the device (directX 11, hardware feature level)
        D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
        HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
                                       0, levels, 2, D3D11_SDK_VERSION,
                                       &m_pDevice, nullptr, &m_pContext);

        if (FAILED(hr))
        {
            CH_ERROR("D3D11CreateDevice failed: 0x%X", hr);
            return false;
        }

        // This stub renderer does not own a swap chain/backbuffer yet.
        // Device/context creation is sufficient for initialization.

        m_initialized = true;
        CH_INFO("D3D11Renderer initialized");
        return true;
    }

    void render() override
    {
        ScopedFrameProfile profile;
        // Present the backbuffer
        // In a real implementation, we'd get the backbuffer and create RTV
        if (m_hWnd)
        {
            // Swap buffers
        }
    }

    void shutdown() override
    {
        m_initialized = false;
        cleanup();
        CH_INFO("D3D11Renderer shutdown");
    }

    void drawLine(const Vec2& p1, const Vec2& p2, uint32_t color, float width) override
    {
        // Shader-based line drawing
        CH_INFO("drawLine stub - p1(%f,%f) p2(%f,%f) color=0x%X", p1.x, p1.y, p2.x, p2.y, color);
    }

    void drawRect(int x, int y, int w, int h, uint32_t color) override
    {
        CH_INFO("drawRect stub - x=%d y=%d w=%d h=%d color=0x%X", x, y, w, h, color);
    }

    void drawText(int x, int y, const std::string& text, uint32_t color, int flags) override
    {
        CH_INFO("drawText stub - x=%d y=%d text='%s' color=0x%X", x, y, text.c_str(), color);
    }

    bool initialized() const override { return m_initialized; }

    FrameStats frameStats() const override
    {
        return g_FrameProfiler().stats();
    }

    void cleanup() override
    {
        // Release D3D resources
        m_pRTV = nullptr;
        m_pDSV = nullptr;
        if (m_pDevice)
        {
            m_pDevice->Release();
            m_pDevice = nullptr;
        }
        if (m_pContext)
        {
            m_pContext->Release();
            m_pContext = nullptr;
        }
    }

private:
    HWND m_hWnd = nullptr;
    ID3D11Device*            m_pDevice = nullptr;
    ID3D11DeviceContext*     m_pContext = nullptr;
    ID3D11RenderTargetView*  m_pRTV = nullptr;
    ID3D11DepthStencilView*  m_pDSV = nullptr;
    bool m_initialized = false;
};

std::unique_ptr<IRenderer> createRenderer()
{
    return std::make_unique<D3D11Renderer>();
}

std::unique_ptr<IRenderer>& g_Renderer()
{
    static std::unique_ptr<IRenderer> s = std::make_unique<D3D11Renderer>();
    return s;
}

} // namespace cheat
