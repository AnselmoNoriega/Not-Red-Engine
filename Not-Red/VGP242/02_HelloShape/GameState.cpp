#include "GameState.h"

using namespace NotRed;
using namespace NotRed::Graphics;
using namespace NotRed::Input;

void MainState::Initialize()
{
	CreateShape();
	
	auto device = GraphicsSystem::Get()->GetDevice();

	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = static_cast<UINT>(mVertices.size() * sizeof(Vertex));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = mVertices.data();

	HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to crate vertex data");

	std::filesystem::path shaderFile = L"../../Assets/Shaders/Basic.fx";

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3DCompileFromFile(
		shaderFile.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_5_0",
		shaderFlags, 0,
		&shaderBlob,
		&errorBlob
	);
	if (errorBlob && errorBlob->GetBufferPointer())
	{
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	}
	ASSERT(SUCCEEDED(hr), "Failed to compile Vertex Shader");

	hr = device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mVertexShader
	);
	ASSERT(SUCCEEDED(hr), "Failed to create Vertex Shader");

	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
	vertexLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT });
	vertexLayout.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT });

	hr = device->CreateInputLayout(
		vertexLayout.data(),
		static_cast<UINT>(vertexLayout.size()),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&mInputLayout
	);
	ASSERT(SUCCEEDED(hr), "Failed to create Input Layout");
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);

	hr = D3DCompileFromFile(
		shaderFile.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		shaderFlags, 0,
		&shaderBlob,
		&errorBlob
	);
	if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
	{
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	}
	ASSERT(SUCCEEDED(hr), "Failed to compile Pixel Shader");

	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader
	);
	ASSERT(SUCCEEDED(hr), "Failed to create Pixel Shader");
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void MainState::Terminate()
{
	mVertices.clear();
	SafeRelease(mPixelShader);
	SafeRelease(mInputLayout);
	SafeRelease(mVertexShader);
	SafeRelease(mVertexBuffer);
}

void MainState::Update(float dt)
{

}

void MainState::Render()
{
	auto context = GraphicsSystem::Get()->GetContext();

	context->VSSetShader(mVertexShader, nullptr, 0);
	context->IASetInputLayout(mInputLayout);
	context->PSSetShader(mPixelShader, nullptr, 0);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	context->Draw(static_cast<UINT>(mVertices.size()), 0);
}

//-------------------Shapes---------------------------------------

void ShapeOne::Update(float dt)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::TWO))
	{
		MainApp().ChangeState("ShapeTwo");
	}
	else if (InputSystem::Get()->IsKeyPressed(KeyCode::THREE))
	{
		MainApp().ChangeState("ShapeThree");
	}
}

void ShapeOne::CreateShape()
{
	mVertices.push_back({ {-0.70f,-0.25f, 0.0f}, Colors::Orange });
	mVertices.push_back({ {-0.50f, 0.40f, 0.0f}, Colors::Orange });
	mVertices.push_back({ { 0.00f,-0.25f, 0.0f}, Colors::Yellow });

	mVertices.push_back({ {-0.50f, 0.40f, 0.0f}, Colors::Orange });
	mVertices.push_back({ {-0.30f, 0.60f, 0.0f}, Colors::Orange });
	mVertices.push_back({ { 0.00f,-0.25f, 0.0f}, Colors::Yellow });

	mVertices.push_back({ {-0.30f, 0.60f, 0.0f}, Colors::Orange });
	mVertices.push_back({ { 0.00f, 0.80f, 0.0f}, Colors::Orange });
	mVertices.push_back({ { 0.00f,-0.25f, 0.0f}, Colors::Yellow });

	mVertices.push_back({ { 0.00f,-0.25f, 0.0f}, Colors::Yellow });
	mVertices.push_back({ { 0.50f, 0.40f, 0.0f}, Colors::Orange });
	mVertices.push_back({ { 0.70f,-0.25f, 0.0f}, Colors::Orange });

	mVertices.push_back({ { 0.00f,-0.25f, 0.0f}, Colors::Yellow });
	mVertices.push_back({ { 0.30f, 0.60f, 0.0f}, Colors::Orange });
	mVertices.push_back({ { 0.50f, 0.40f, 0.0f}, Colors::Orange });

	mVertices.push_back({ { 0.00f,-0.25f, 0.0f}, Colors::Yellow });
	mVertices.push_back({ { 0.00f, 0.80f, 0.0f}, Colors::Orange });
	mVertices.push_back({ { 0.30f, 0.60f, 0.0f}, Colors::Orange });

	mVertices.push_back({ {-0.50f,-0.25f, 0.0f}, Colors::DarkGreen });
	mVertices.push_back({ {-0.25f, 0.25f, 0.0f}, Colors::Cyan });
	mVertices.push_back({ { 0.00f,-0.25f, 0.0f}, Colors::DarkGreen });

	mVertices.push_back({ { 0.00f,-0.25f, 0.0f}, Colors::DarkGreen });
	mVertices.push_back({ { 0.25f, 0.25f, 0.0f}, Colors::Cyan });
	mVertices.push_back({ { 0.50f,-0.25f, 0.0f}, Colors::DarkGreen });

	mVertices.push_back({ {-0.25f,-0.25f, 0.0f}, Colors::Brown });
	mVertices.push_back({ { 0.00f, 0.25f, 0.0f}, Colors::DarkGreen });
	mVertices.push_back({ { 0.25f,-0.25f, 0.0f}, Colors::Brown });

	mVertices.push_back({ {-0.75f,-0.25f, 0.0f}, Colors::Brown });
	mVertices.push_back({ {-0.60f, 0.12f, 0.0f}, Colors::DarkGreen });
	mVertices.push_back({ {-0.25f,-0.25f, 0.0f}, Colors::Brown });

	mVertices.push_back({ { 0.25f,-0.25f, 0.0f}, Colors::Brown });
	mVertices.push_back({ { 0.55f, 0.20f, 0.0f}, Colors::DarkGreen });
	mVertices.push_back({ { 0.75f,-0.25f, 0.0f}, Colors::Brown });
}

void ShapeTwo::Update(float dt)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::ONE))
	{
		MainApp().ChangeState("ShapeOne");
	}
	else if (InputSystem::Get()->IsKeyPressed(KeyCode::THREE))
	{
		MainApp().ChangeState("ShapeThree");
	}
}

void ShapeTwo::CreateShape()
{
	mVertices.push_back({ { 0.00f,-0.50f, 0.0f}, Colors::Black });
	mVertices.push_back({ {-0.20f, 0.50f, 0.0f}, Colors::OrangeRed });
	mVertices.push_back({ { 0.00f, 0.50f, 0.0f}, Colors::OrangeRed });

	mVertices.push_back({ { 0.00f,-0.50f, 0.0f}, Colors::Black });
	mVertices.push_back({ { 0.00f, 0.50f, 0.0f}, Colors::OrangeRed });
	mVertices.push_back({ { 0.20f, 0.50f, 0.0f}, Colors::OrangeRed });

	mVertices.push_back({ {-0.10f, 0.00f, 0.0f}, Colors::Black });
	mVertices.push_back({ {-0.40f, 0.40f, 0.0f}, Colors::DarkOrange });
	mVertices.push_back({ {-0.20f, 0.50f, 0.0f}, Colors::DarkOrange });

	mVertices.push_back({ { 0.10f, 0.00f, 0.0f}, Colors::Black });
	mVertices.push_back({ { 0.20f, 0.50f, 0.0f}, Colors::DarkOrange });
	mVertices.push_back({ { 0.40f, 0.40f, 0.0f}, Colors::DarkOrange });

	mVertices.push_back({ { 0.00f,-0.50f, 0.0f}, Colors::Black });
	mVertices.push_back({ {-0.25f, 0.20f, 0.0f}, Colors::Pink });
	mVertices.push_back({ {-0.10f, 0.00f, 0.0f}, Colors::Pink });

	mVertices.push_back({ { 0.00f,-0.50f, 0.0f}, Colors::Black });
	mVertices.push_back({ { 0.10f, 0.00f, 0.0f}, Colors::Pink });
	mVertices.push_back({ { 0.25f, 0.20f, 0.0f}, Colors::Pink });

	mVertices.push_back({ {-0.055f,-0.35f, 0.0f}, Colors::Black });
	mVertices.push_back({ {-0.55f , 0.10f, 0.0f}, Colors::DarkSlateGray });
	mVertices.push_back({ {-0.25f , 0.20f, 0.0f}, Colors::DarkSlateGray });

	mVertices.push_back({ { 0.055f,-0.35f, 0.0f}, Colors::Black });
	mVertices.push_back({ { 0.25f, 0.20f, 0.0f}, Colors::DarkSlateGray });
	mVertices.push_back({ { 0.55f, 0.10f, 0.0f}, Colors::DarkSlateGray });

	mVertices.push_back({ {-0.25f, 0.20f, 0.0f}, Colors::Black });
	mVertices.push_back({ {-0.55f, 0.10f, 0.0f}, Colors::DarkOrange });
	mVertices.push_back({ {-0.40f, 0.40f, 0.0f}, Colors::DarkOrange });

	mVertices.push_back({ { 0.25f, 0.20f, 0.0f}, Colors::Black });
	mVertices.push_back({ { 0.40f, 0.40f, 0.0f}, Colors::DarkOrange });
	mVertices.push_back({ { 0.55f, 0.10f, 0.0f}, Colors::DarkOrange });

	mVertices.push_back({ {-0.20f, 0.50f, 0.0f}, Colors::DarkOrange });
	mVertices.push_back({ {-0.40f, 0.40f, 0.0f}, Colors::DarkOrange });
	mVertices.push_back({ {-0.35f, 0.70f, 0.0f}, Colors::AntiqueWhite });

	mVertices.push_back({ { 0.20f, 0.50f, 0.0f}, Colors::DarkOrange });
	mVertices.push_back({ { 0.35f, 0.70f, 0.0f}, Colors::AntiqueWhite });
	mVertices.push_back({ { 0.40f, 0.40f, 0.0f}, Colors::DarkOrange });
}

void ShapeThree::Update(float dt)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::ONE))
	{
		MainApp().ChangeState("ShapeOne");
	}
	else if (InputSystem::Get()->IsKeyPressed(KeyCode::TWO))
	{
		MainApp().ChangeState("ShapeTwo");
	}
}

void ShapeThree::CreateShape()
{
	mVertices.push_back({ {-0.50f, 0.00f, 0.0f}, Colors::DarkBlue });
	mVertices.push_back({ {-0.02f,-0.10f, 0.0f}, Colors::Blue });
	mVertices.push_back({ {-0.02f,-0.50f, 0.0f}, Colors::DarkBlue });

	mVertices.push_back({ { 0.02f,-0.10f, 0.0f}, Colors::Red });
	mVertices.push_back({ { 0.50f, 0.00f, 0.0f}, Colors::DarkRed });
	mVertices.push_back({ { 0.02f,-0.50f, 0.0f}, Colors::DarkRed });

	mVertices.push_back({ {-0.05f,-0.05f, 0.0f}, Colors::LightGreen });
	mVertices.push_back({ {-0.50f, 0.05f, 0.0f}, Colors::Green });
	mVertices.push_back({ {-0.40f, 0.40f, 0.0f}, Colors::Green });

	mVertices.push_back({ { 0.05f,-0.05f, 0.0f}, Colors::LightPink });
	mVertices.push_back({ { 0.40f, 0.40f, 0.0f}, Colors::HotPink });
	mVertices.push_back({ { 0.50f, 0.05f, 0.0f}, Colors::HotPink });

	mVertices.push_back({ { 0.02f,-0.03f, 0.0f}, Colors::Orange });
	mVertices.push_back({ { 0.02f, 0.75f, 0.0f}, Colors::OrangeRed });
	mVertices.push_back({ { 0.38f, 0.43f, 0.0f}, Colors::OrangeRed });

	mVertices.push_back({ {-0.02f,-0.03f, 0.0f}, Colors::LightYellow });
	mVertices.push_back({ {-0.38f, 0.43f, 0.0f}, Colors::Yellow });
	mVertices.push_back({ {-0.02f, 0.75f, 0.0f}, Colors::Yellow });
}


