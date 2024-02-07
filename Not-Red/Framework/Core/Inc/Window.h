#pragma once

namespace NotRed::Core
{
	class Window
	{
	public:
		void Initialize(HINSTANCE instance, const std::wstring& appName, uint32_t width, uint32_t height);
		void Terminate();

		void ProcessMessage();

		HWND GetWindowHandle() const { return mWindow; }
		bool IsActive() const { return mActive; }

	private:
		HINSTANCE mInstance = nullptr;
		HWND mWindow = nullptr;
		RECT mScreenRect{};
		std::wstring mAppName;
		bool mActive = true;
	};
}