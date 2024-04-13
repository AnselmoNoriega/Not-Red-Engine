#include "Precompiled.h"
#include "ConstantBuffer.h"

#include "GraphicsSystem.h"

using namespace NotRed;
using namespace NotRed::Graphics;

ConstantBuffer::~ConstantBuffer()
{
	ASSERT(mConstantBuffer == nullptr, "ConstantBuffer: terminate must be called");
}

void ConstantBuffer::Initialize(uint32_t bufferSize)
{
	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = bufferSize;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	auto device = GraphicsSystem::Get()->GetDevice();
	HRESULT hr = device->CreateBuffer(&desc, nullptr, &mConstantBuffer);
	ASSERT(SUCCEEDED(hr), "ConstantBuffer: Failed to create buffer");
}

void ConstantBuffer::Terminate()
{
	SafeRelease(mConstantBuffer);
}

void ConstantBuffer::Update(const void* data)
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->UpdateSubresource(mConstantBuffer, 0, nullptr, data, 0, 0);
}

void ConstantBuffer::BindVS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->VSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void ConstantBuffer::BindPS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->PSSetConstantBuffers(slot, 1, &mConstantBuffer);
}
