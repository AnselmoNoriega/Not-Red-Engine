#pragma once

#include <Not-Red/Inc/NotRed.h>

class MainState : public NotRed::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float dt) override;
	void Render() override;

protected:
	void virtual CreateShape() {};

	struct Vertex
	{
		NotRed::Math::Vector3 Position;
		NotRed::Color Color;
	};

	using Vertices = std::vector<Vertex>;

	Vertices mVertices;
	ID3D11Buffer* mVertexBuffer = nullptr;
	ID3D11VertexShader* mVertexShader = nullptr;
	ID3D11InputLayout* mInputLayout = nullptr;
	ID3D11PixelShader* mPixelShader = nullptr;
};

class ShapeOne : public MainState
{
public:
	void Update(float dt) override;

protected:
	void CreateShape() override;
};

class ShapeTwo : public MainState
{
public:
	void Update(float dt) override;

protected:
	void CreateShape() override;
};

class ShapeThree : public MainState
{
public:
	void Update(float dt) override;

protected:
	void CreateShape() override;
};
