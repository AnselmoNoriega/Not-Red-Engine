#include "Precompiled.h"
#include "GeometryShader.h"
#include "GraphicsSystem.h"

using namespace NotRed;
using namespace NotRed::Graphics;

void GeometryShader::Initialize(const std::filesystem::path& filePath)
{
	auto device = GraphicsSystem::Get()->GetDevice();

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3DCompileFromFile(
		filePath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"GS", "gs_5_0",
		shaderFlags, 0,
		&shaderBlob,
		&errorBlob
	);
	if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
	{
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	}
	ASSERT(SUCCEEDED(hr), "Failed to compile pixel shader");

	hr = device->CreateGeometryShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mGeometryShader
	);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader");
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void GeometryShader::Terminate()
{
	SafeRelease(mGeometryShader);
}

void GeometryShader::Bind()
{
	auto context = GraphicsSystem::Get()->GetContext();

	context->GSSetShader(mGeometryShader, nullptr, 0);
}

void GeometryShader::Unbind()
{
	static ID3D11ShaderResourceView* dummy = nullptr;
	GraphicsSystem::Get()->GetContext()->GSSetShader(nullptr, nullptr, 0);
}
