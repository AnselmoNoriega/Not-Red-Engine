#include "Precompiled.h"
#include "GraphicsSystem.h"

using namespace NotRed;
using namespace NotRed::Graphics;
using namespace NotRed::Core;

namespace
{
    std::unique_ptr<GraphicsSystem> sGraphicsSystem;
    WindowMessageHandler sWindowMessageHandler;
}

LRESULT GraphicsSystem::GraphicsSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (sGraphicsSystem)
    {
        switch (message)
        {
        case WM_SIZE:
        {
            const uint32_t width = static_cast<uint32_t>(LOWORD(lParam));
            const uint32_t height = static_cast<uint32_t>(HIWORD(lParam));
            sGraphicsSystem->Resize(width, height);
            break;
        }
        }
    }

    return sWindowMessageHandler.ForwardMessage(window, message, wParam, lParam);
}

void GraphicsSystem::StaticInitialize(HWND window, bool fullscreen)
{
    ASSERT(sGraphicsSystem == nullptr, "GraphicsSystem is already initialized");
    sGraphicsSystem = std::make_unique<GraphicsSystem>();
    sGraphicsSystem->Initialize(window, fullscreen);
}

void GraphicsSystem::StaticTerminate()
{
    if (sGraphicsSystem)
    {
        sGraphicsSystem->Terminate();
        sGraphicsSystem.reset();
    }
}

GraphicsSystem* GraphicsSystem::Get()
{
    ASSERT(sGraphicsSystem, "GraphicsSystem: is not initialized");
    return sGraphicsSystem.get();
}

GraphicsSystem::~GraphicsSystem()
{
    ASSERT(mD3DDevice == nullptr, "GraphicsSystem: must be terminated");
}

void GraphicsSystem::Initialize(HWND window, bool fullscreen)
{
    // Initialize window dimensions
    RECT clientRect = {};
    GetClientRect(window, &clientRect);
    UINT width = static_cast<uint32_t>(clientRect.right - clientRect.left);
    UINT height = static_cast<uint32_t>(clientRect.bottom - clientRect.top);

    // Create swap chain description
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = window;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = !fullscreen;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    const D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

    // Create device and swap chain
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        &featureLevel,
        1,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &mSwapChain,
        &mD3DDevice,
        nullptr,
        &mImmediateContext
    );

    // Check initialization success
    ASSERT(SUCCEEDED(hr), "GraphicsSystem failed to initialize device or swap chain");
    mSwapChain->GetDesc(&mSwapChainDesc);

    // Resize buffers
    Resize(GetBackBufferWidth(), GetBackBufferHeight());

    // Create Depth-Stencil State
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; // Standard depth test
    depthStencilDesc.StencilEnable = FALSE; // We don't need stencil testing for now
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // Create the Depth-Stencil State (using ComPtr)
    hr = mD3DDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
    ASSERT(SUCCEEDED(hr), "Failed to create depth stencil state");

    // Set Depth-Stencil State
    mImmediateContext->OMSetDepthStencilState(mDepthStencilState, 1);

    // Create render target view (back buffer)
    ID3D11Texture2D* backBuffer = nullptr;
    hr = mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    ASSERT(SUCCEEDED(hr), "GraphicsSystem failed to get back buffer");

    hr = mD3DDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView);
    SafeRelease(backBuffer);
    ASSERT(SUCCEEDED(hr), "GraphicsSystem failed to create render target");

    // Create depth-stencil texture and view
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = GetBackBufferWidth();
    descDepth.Height = GetBackBufferHeight();
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D32_FLOAT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;

    hr = mD3DDevice->CreateTexture2D(&descDepth, nullptr, &mDepthStencilBuffer);
    ASSERT(SUCCEEDED(hr), "GraphicsSystem failed to create depth stencil buffer");

    hr = mD3DDevice->CreateDepthStencilView(mDepthStencilBuffer, nullptr, &mDepthStencilView);
    ASSERT(SUCCEEDED(hr), "GraphicsSystem failed to create depth stencil view");

    // Set render targets
    mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

    // Set up viewport
    mViewport.Width = static_cast<float>(GetBackBufferWidth());
    mViewport.Height = static_cast<float>(GetBackBufferHeight());
    mViewport.MinDepth = 0.0f;
    mViewport.MaxDepth = 1.0f;
    mViewport.TopLeftX = 0;
    mViewport.TopLeftY = 0;
    mImmediateContext->RSSetViewports(1, &mViewport);

    // Hook window message handler
    sWindowMessageHandler.Hook(window, GraphicsSystemMessageHandler);
}


void GraphicsSystem::Terminate()
{
    sWindowMessageHandler.Unhook();

    SafeRelease(mDepthStencilView);
    SafeRelease(mDepthStencilState);
    SafeRelease(mDepthStencilBuffer);
    SafeRelease(mRenderTargetView);
    SafeRelease(mSwapChain);
    SafeRelease(mImmediateContext);
    SafeRelease(mD3DDevice);
}

void GraphicsSystem::BeginRender()
{
    mImmediateContext->OMSetDepthStencilState(mDepthStencilState, 0);

    mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
    mImmediateContext->ClearRenderTargetView(mRenderTargetView, (FLOAT*)(&mClearColor));
    mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
}

void GraphicsSystem::EndRender()
{
    mSwapChain->Present(mVSync, 0);
}

void GraphicsSystem::ToggleFullScreen()
{
    BOOL fullscreen;
    mSwapChain->GetFullscreenState(&fullscreen, nullptr);
    mSwapChain->SetFullscreenState(!fullscreen, nullptr);
}

void GraphicsSystem::Resize(uint32_t width, uint32_t height)
{
    mImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);

    SafeRelease(mRenderTargetView);
    SafeRelease(mDepthStencilView);
    SafeRelease(mDepthStencilBuffer);

    HRESULT hr;
    if (width != GetBackBufferWidth() || height != GetBackBufferHeight())
    {
        hr = mSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
        ASSERT(SUCCEEDED(hr), "GraphicsSystem failed to access the swap chain view");

        mSwapChain->GetDesc(&mSwapChainDesc);
    }

    ID3D11Texture2D* backBuffer = nullptr;
    hr = mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    ASSERT(SUCCEEDED(hr), "GraphicsSystem failed to get back buffer");

    hr = mD3DDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView);
    SafeRelease(backBuffer);
    ASSERT(SUCCEEDED(hr), "GraphicsSystem failed to create render target");

    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = GetBackBufferWidth();
    descDepth.Height = GetBackBufferHeight();
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D32_FLOAT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = mD3DDevice->CreateTexture2D(&descDepth, nullptr, &mDepthStencilBuffer);
    ASSERT(SUCCEEDED(hr), "GraphicsSystem failed to create stencil buffer");

    hr = mD3DDevice->CreateDepthStencilView(mDepthStencilBuffer, nullptr, &mDepthStencilView);
    ASSERT(SUCCEEDED(hr), "GraphicsSystem failed to create depth stencil view");

    mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

    mViewport.Width = static_cast<float>(GetBackBufferWidth());
    mViewport.Height = static_cast<float>(GetBackBufferHeight());
    mViewport.MinDepth = 0.0f;
    mViewport.MaxDepth = 1.0f;
    mViewport.TopLeftX = 0;
    mViewport.TopLeftY = 0;
    mImmediateContext->RSSetViewports(1, &mViewport);
}

void GraphicsSystem::ResetRenderTarget()
{
    mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}

void GraphicsSystem::ResetViewport()
{
    mImmediateContext->RSSetViewports(1, &mViewport);
}

void GraphicsSystem::SetClearColor(const Color& color)
{
    mClearColor = color;
}

void GraphicsSystem::SetVSync(bool vSync)
{
    mVSync = vSync ? 1 : 0;
}

uint32_t GraphicsSystem::GetBackBufferWidth() const
{
    return mSwapChainDesc.BufferDesc.Width;
}

uint32_t GraphicsSystem::GetBackBufferHeight() const
{
    return mSwapChainDesc.BufferDesc.Height;
}

float GraphicsSystem::GetBackBufferAspectRatio() const
{
    return static_cast<float>(GetBackBufferWidth()) / static_cast<float>(GetBackBufferHeight());
}
