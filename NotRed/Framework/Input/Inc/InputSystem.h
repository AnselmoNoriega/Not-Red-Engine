#pragma once

#include "InputTypes.h"

namespace NotRed::Input
{
	class InputSystem final
	{
	public:
		static void StaticInitialize(HWND window);
		static void StaticTerminate();
		static InputSystem* Get();

	public:
		InputSystem() = default;
		~InputSystem();

		InputSystem(const InputSystem&) = delete;
		InputSystem& operator=(const InputSystem&) = delete;

		void Initialize(HWND window);
		void Terminate();

		void Update();

		bool IsKeyDown(KeyCode key) const;
		bool IsKeyPressed(KeyCode key) const;

		bool IsMouseDown(MouseButton button) const;
		bool IsMousePressed(MouseButton button) const;

		int GetMouseMoveX() const;
		int GetMouseMoveY() const;
		float GetMouseMoveZ() const;

		int GetMouseScreenX() const;
		int GetMouseScreenY() const;

		bool IsMouseLeftEdge() const;
		bool IsMouseRightEdge() const;
		bool IsMouseTopEdge() const;
		bool IsMouseBottomEdge() const;

		void ShowSystemCursor(bool show);
		void SetMouseClipToWindow(bool clip);
		bool IsMouseClipToWindow() const;

	private:
		static LRESULT CALLBACK InputSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

		HWND mWindow = nullptr;

		bool mCurrKeys[512]{};
		bool mPrevKeys[512]{};
		bool mPressedKeys[512]{};

		bool mClipMouseToWindow = false;

		int mCurrMouseX = -1;
		int mCurrMouseY = -1;
		int mPrevMouseX = -1;
		int mPrevMouseY = -1;
		int mMouseMoveX = 0;
		int mMouseMoveY = 0;

		float mMouseWheel = 0.0f;

		bool mCurrMouseButtons[3]{};
		bool mPrevMouseButtons[3]{};
		bool mPressedMouseButtons[3]{};

		bool mMouseLeftEdge = false;
		bool mMouseRightEdge = false;
		bool mMouseTopEdge = false;
		bool mMouseBottomEdge = false;

		bool mInitialized = false;
	};
}