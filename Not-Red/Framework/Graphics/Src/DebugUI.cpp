#include "Precompiled.h"
#include "DebugUI.h"

#include "GraphicsSystem.h"
#include <ImGui/Inc/imgui_impl_dx11.h>
#include <ImGui/Inc/imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

using namespace NotRed;
using namespace NotRed::Graphics;
using namespace NotRed::Core;

namespace
{
	WindowMessageHandler sWindowMessageHandler;

	bool IsMouseInput(UINT message)
	{
		switch (message)
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONDBLCLK:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_MOUSEHWHEEL:
			return true;
		default:
			return false;
		}
	}

	bool IsKeyboardInput(UINT message)
	{
		switch (message)
		{
		case WM_CHAR:
		case WM_KEYUP:
		case WM_KEYDOWN:
		case WM_SYSKEYUP:
		case WM_SYSKEYDOWN:
			return true;
		default:
			return false;
		}
	}

	LRESULT CALLBACK DebugUIMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
	{
		ImGuiIO& io = ImGui::GetIO();

		if (io.WantCaptureMouse && IsMouseInput(message))
		{
			return ImGui_ImplWin32_WndProcHandler(window, message, wParam, lParam);
		}

		if (io.WantCaptureKeyboard && IsKeyboardInput(message))
		{
			return ImGui_ImplWin32_WndProcHandler(window, message, wParam, lParam);
		}

		LRESULT result = ImGui_ImplWin32_WndProcHandler(window, message, wParam, lParam);
		if (result != 0)
		{
			return result;
		}

		return sWindowMessageHandler.ForwardMessage(window, message, wParam, lParam);
	}
}

void DebugUI::StaticInitialize(HWND window, bool docking, bool multiViewport)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	if (docking)
	{
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}
	if (multiViewport)
	{
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	}

	auto device = GraphicsSystem::Get()->GetDevice();
	auto context = GraphicsSystem::Get()->GetContext();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, context);

	sWindowMessageHandler.Hook(window, DebugUIMessageHandler);
}

void DebugUI::StaticTerminate()
{
	sWindowMessageHandler.Unhook();
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DebugUI::SetTheme(Theme theme)
{
	switch (theme)
	{
	case Theme::Classic:
	{
		ImGui::StyleColorsClassic();
		break;
	}
	case Theme::Dark:
	{
		ImGui::StyleColorsDark();
		break;
	}
	case Theme::Light:
	{
		ImGui::StyleColorsLight();
		break;
	}
	case Theme::Custom:
	{
		ImGui::StyleColorsDark();
		auto& style = ImGui::GetStyle();
		style.WindowRounding = 10.0f;
		style.Colors[ImGuiCol_WindowBg] = { 0.09f, 0.0f, 0.0f, 0.5f };
		style.Colors[ImGuiCol_PopupBg] = { 0.09f, 0.0f, 0.0f, 0.9f };
		style.Colors[ImGuiCol_ChildBg] = { 0.15f, 0.0f, 0.0f, 0.5f };
		style.Colors[ImGuiCol_FrameBg] = { 0.35f, 0.0f, 0.15f, 0.5f };
		style.Colors[ImGuiCol_Header] = { 0.8f, 0.1f, 0.0f, 0.5f };
		style.Colors[ImGuiCol_CheckMark] = { 0.8f, 0.1f, 0.8f, 0.5f };
		style.Colors[ImGuiCol_FrameBgHovered] = { 0.8f, 0.1f, 0.3f, 0.5f };
		style.Colors[ImGuiCol_FrameBgActive] = { 0.8f, 0.1f, 0.4f, 0.6f };
		style.Colors[ImGuiCol_HeaderHovered] = { 0.8f, 0.1f, 0.3f, 0.7f };
		style.Colors[ImGuiCol_ResizeGrip] = { 0.35f, 0.0f, 0.0f, 0.7f };
		style.Colors[ImGuiCol_TitleBgActive] = { 0.2f, 0.0f, 0.05f, 0.9f };
		style.Colors[ImGuiCol_TitleBgCollapsed] = { 0.2f, 0.0f, 0.05f, 0.9f };
		style.Colors[ImGuiCol_NavHighlight] = { 0.2f, 0.0f, 0.05f, 0.9f };
		style.Colors[ImGuiCol_Border] = { 1.0f, 1.0f, 1.0f, 0.6f };
		style.Colors[ImGuiCol_Button] = { 0.5f, 0.0f, 0.05f, 0.9f };
		//style.Colors[ImGuiCol_Button] = { 1.0f, 1.0f, 1.0f, 1.0f };
		break;
	}
	}
}

void DebugUI::BeginRender()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void DebugUI::EndRender()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}
