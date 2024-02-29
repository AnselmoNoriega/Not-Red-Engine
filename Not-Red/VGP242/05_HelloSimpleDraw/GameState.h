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
	virtual void CreateShape() {};

	NotRed::Graphics::Camera mCamera;
};
