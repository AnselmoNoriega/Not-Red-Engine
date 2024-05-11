#include "Precompiled.h"
#include "Texture.h"

#include "GraphicsSystem.h"
#include <DirectXTK/Inc/WICTextureLoader.h>

using namespace NotRed;
using namespace NotRed::Graphics;

void NotRed::Graphics::Texture::UnbindPS(uint32_t slot)
{
    static ID3D11ShaderResourceView* dummy = nullptr;
    GraphicsSystem::Get()->GetContext()->PSSetShaderResources(slot, 1, &dummy);
}

Texture::~Texture()
{
    ASSERT(mShaderResourceView == nullptr, "Texture: This is not empty (Call Terminate before)");
}

Texture::Texture(Texture&& rhs) noexcept
    : mShaderResourceView(rhs.mShaderResourceView)
{
    rhs.mShaderResourceView = nullptr;
}

Texture& Texture::operator=(Texture&& rhs) noexcept
{
    mShaderResourceView = rhs.mShaderResourceView;
    rhs.mShaderResourceView = nullptr;
    return *this;
}

void Texture::Initialize(const std::filesystem::path& fileName)
{
    auto device = GraphicsSystem::Get()->GetDevice();
    auto context = GraphicsSystem::Get()->GetContext();
    HRESULT hr = DirectX::CreateWICTextureFromFile(device, context, fileName.c_str(), nullptr, &mShaderResourceView);
    ASSERT(SUCCEEDED(hr), "Texture: failed to create texture %ls.", fileName.c_str());
}

void Texture::Initialize(uint32_t width, uint32_t height, Format format)
{
    ASSERT(false, "Texture: not yet implemented");
}

void Texture::Terminate()
{
    SafeRelease(mShaderResourceView);
}

void Texture::BindVS(uint32_t slot) const
{
    auto context = GraphicsSystem::Get()->GetContext();
    context->VSSetShaderResources(slot, 1, &mShaderResourceView);
}

void Texture::BindPS(uint32_t slot) const
{
    auto context = GraphicsSystem::Get()->GetContext();
    context->PSSetShaderResources(slot, 1, &mShaderResourceView);
}

DXGI_FORMAT Texture::GetDXGIFormat(Format format)
{
    switch (format)
    {
    case Format::RGBA_U8: return DXGI_FORMAT_R8G8B8A8_UNORM;
    case Format::RGBA_U32: return DXGI_FORMAT_R32G32B32A32_UINT;
    default:
        return DXGI_FORMAT_R8G8B8A8_UNORM;
    }
}
