#pragma once

#include <Not-Red/Inc/NotRed.h>

class MainState : public NotRed::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float dt) override;
	void Render() override;
	void DebugUI() override;

private:
	virtual void CreateShape() {};

	NotRed::Graphics::Camera mCamera;

	bool drawPlane = true;
	Color shapeColor = Colors::Green;
	Math::Vector3 shapePos;

	int shapeType = 1;
};
