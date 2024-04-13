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
	virtual void DrawShape() {};

	NotRed::Graphics::Camera mCamera;
};

using namespace NotRed::Graphics;

class Sphere : public MainState
{
protected:
	void DrawShape() override { SimpleDraw::AddSphere(30, 30, 10, Colors::Red); }
};

class Transform : public MainState
{
protected:
	void DrawShape() override { SimpleDraw::AddTransform(Math::Matrix4::Identity); }
};

class AABB : public MainState
{
protected:
	void DrawShape() override { SimpleDraw::AddAABB(-5.0f, -5.0f, -5.0f, 5.0f, 5.0f, 5.0f, Colors::Yellow); }
};

class FilledAABB : public MainState
{
protected:
	void DrawShape() override { SimpleDraw::AddFilledAABB(-5.0f, -5.0f, -5.0f, 5.0f, 5.0f, 5.0f, Colors::Green); }
};

class Line : public MainState
{
protected:
	void DrawShape() override { SimpleDraw::AddLine({ -10.0f, 5.0f, 0.0f }, { 10.0f, 5.0f, 0.0f }, Colors::BlueViolet); }
};