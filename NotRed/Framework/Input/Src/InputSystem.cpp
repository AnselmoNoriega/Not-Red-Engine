#include "Precompiled.h"
#include "InputSystem.h"

using namespace NotRed;
using namespace NotRed::Input;

namespace
{
	std::unique_ptr<InputSystem> sInputSystem;
	Core::WindowMessageHandler sWindowMessageHandler;

	void ClipToWindow(HWND window)
	{
		RECT rect;
		GetClientRect(window, &rect);

		POINT ul;
		ul.x = rect.left;
		ul.y = rect.top;

		POINT lr;
		lr.x = rect.right;
		lr.y = rect.bottom;

		MapWindowPoints(window, nullptr, &ul, 1);
		MapWindowPoints(window, nullptr, &lr, 1);

		rect.left = ul.x;
		rect.top = ul.y;

		rect.right = lr.x;
		rect.bottom = lr.y;

		ClipCursor(&rect);
	}
}

LRESULT CALLBACK InputSystem::InputSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (sInputSystem)
	{
		switch (message)
		{
			case WM_ACTIVATEAPP:
			{
				if (wParam == TRUE)
				{
					SetCapture(window);
				}
				else
				{
					sInputSystem->mMouseLeftEdge = false;
					sInputSystem->mMouseRightEdge = false;
					sInputSystem->mMouseTopEdge = false;
					sInputSystem->mMouseBottomEdge = false;
					ReleaseCapture();
				}
				break;
			}
			case WM_LBUTTONDOWN:
			{
				sInputSystem->mCurrMouseButtons[0] = true;
				break;
			}
			case WM_LBUTTONUP:
			{
				sInputSystem->mCurrMouseButtons[0] = false;
				break;
			}
			case WM_RBUTTONDOWN:
			{
				sInputSystem->mCurrMouseButtons[1] = true;
				break;
			}
			case WM_RBUTTONUP:
			{
				sInputSystem->mCurrMouseButtons[1] = false;
				break;
			}
			case WM_MBUTTONDOWN:
			{
				sInputSystem->mCurrMouseButtons[2] = true;
				break;
			}
			case WM_MBUTTONUP:
			{
				sInputSystem->mCurrMouseButtons[2] = false;
				break;
			}
			case WM_MOUSEWHEEL:
			{
				sInputSystem->mMouseWheel += (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
				break;
			}
			case WM_MOUSEMOVE:
			{
				int mouseX = (signed short)(lParam);
				int mouseY = (signed short)(lParam >> 16);

				sInputSystem->mCurrMouseX = mouseX;
				sInputSystem->mCurrMouseY = mouseY;
				if (sInputSystem->mPrevMouseX == -1)
				{
					sInputSystem->mPrevMouseX = mouseX;
					sInputSystem->mPrevMouseY = mouseY;
				}

				RECT rect;
				GetClientRect(window, &rect);
				sInputSystem->mMouseLeftEdge = mouseX <= rect.left;
				sInputSystem->mMouseRightEdge = mouseX + 1 >= rect.right;
				sInputSystem->mMouseTopEdge = mouseY <= rect.top;
				sInputSystem->mMouseBottomEdge = mouseY + 1 >= rect.bottom;
				break;
			}
			case WM_KEYDOWN:
			{
				if (wParam < 256)
				{
					sInputSystem->mCurrKeys[wParam] = true;
				}
				break;
			}
			case WM_KEYUP:
			{
				if (wParam < 256)
				{
					sInputSystem->mCurrKeys[wParam] = false;
				}
				break;
			}
		}
	}

	return sWindowMessageHandler.ForwardMessage(window, message, wParam, lParam);
}

void InputSystem::StaticInitialize(HWND window)
{
	ASSERT(sInputSystem == nullptr, "InputSystem -- System already initialized!");
	sInputSystem = std::make_unique<InputSystem>();
	sInputSystem->Initialize(window);
}

void InputSystem::StaticTerminate()
{
	if (sInputSystem != nullptr)
	{
		sInputSystem->Terminate();
		sInputSystem.reset();
	}
}

InputSystem* InputSystem::Get()
{
	ASSERT(sInputSystem != nullptr, "InputSystem -- No system registered.");
	return sInputSystem.get();
}

InputSystem::~InputSystem()
{
	ASSERT(!mInitialized, "InputSystem -- Terminate() must be called to clean up!");
}

void InputSystem::Initialize(HWND window)
{
	// Check if we have already initialized the system
	if (mInitialized)
	{
		LOG("InputSystem -- System already initialized.");
		return;
	}

	LOG("InputSystem -- Initializing...");
	
	// Hook application to window's procedure
	sWindowMessageHandler.Hook(window, InputSystemMessageHandler);

	mInitialized = true;

	LOG("InputSystem -- System initialized.");
}

void InputSystem::Terminate()
{
	// Check if we have already terminated the system
	if (!mInitialized)
	{
		LOG("InputSystem -- System already terminated.");
		return;
	}

	LOG("InputSystem -- Terminating...");

	//mGamePad.reset();
	mInitialized = false;

	// Restore original window's procedure
	sWindowMessageHandler.Unhook();

	LOG("InputSystem -- System terminated.");
}

void InputSystem::Update()
{
	ASSERT(mInitialized, "InputSystem -- System not initialized.");

	// Store the previous keyboard state
	for (int i = 0; i < 512; ++i)
	{
		mPressedKeys[i] = !mPrevKeys[i] && mCurrKeys[i];
	}
	memcpy(mPrevKeys, mCurrKeys, sizeof(mCurrKeys));

	// Update mouse movement
	mMouseMoveX = mCurrMouseX - mPrevMouseX;
	mMouseMoveY = mCurrMouseY - mPrevMouseY;
	mPrevMouseX = mCurrMouseX;
	mPrevMouseY = mCurrMouseY;

	// Store the previous mouse state
	for (int i = 0; i < 3; ++i)
	{
		mPressedMouseButtons[i] = !mPrevMouseButtons[i] && mCurrMouseButtons[i];
	}
	memcpy(mPrevMouseButtons, mCurrMouseButtons, sizeof(mCurrMouseButtons));
}

bool InputSystem::IsKeyDown(KeyCode key) const
{
	return mCurrKeys[(int)key];
}

bool InputSystem::IsKeyPressed(KeyCode key) const
{
	return mPressedKeys[(int)key];
}

bool InputSystem::IsMouseDown(MouseButton button) const
{
	return mCurrMouseButtons[(int)button];
}

bool InputSystem::IsMousePressed(MouseButton button) const
{
	return mPressedMouseButtons[(int)button];
}

int InputSystem::GetMouseMoveX() const
{
	return mMouseMoveX;
}

int InputSystem::GetMouseMoveY() const
{
	return mMouseMoveY;
}

float InputSystem::GetMouseMoveZ() const
{
	return mMouseWheel;
}

int InputSystem::GetMouseScreenX() const
{
	return mCurrMouseX;
}

int InputSystem::GetMouseScreenY() const
{
	return mCurrMouseY;
}

bool InputSystem::IsMouseLeftEdge() const
{
	return mMouseLeftEdge;
}

bool InputSystem::IsMouseRightEdge() const
{
	return mMouseRightEdge;
}

bool InputSystem::IsMouseTopEdge() const
{
	return mMouseTopEdge;
}

bool InputSystem::IsMouseBottomEdge() const
{
	return mMouseBottomEdge;
}

void InputSystem::ShowSystemCursor(bool show)
{
	ShowCursor(show);
}

void InputSystem::SetMouseClipToWindow(bool clip)
{
	mClipMouseToWindow = clip;
}

bool InputSystem::IsMouseClipToWindow() const
{
	return mClipMouseToWindow;
}