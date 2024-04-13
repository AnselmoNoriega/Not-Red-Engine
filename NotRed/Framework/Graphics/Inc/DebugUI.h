#pragma once

namespace NotRed::Graphics::DebugUI
{
	enum class Theme
	{
		Classic,
		Dark,
		Light,
		Custom
	};

	void StaticInitialize(HWND window, bool docking = false, bool multiViewport = false);
	void StaticTerminate();

	void SetTheme(Theme theme);

	void BeginRender();
	void EndRender();
}