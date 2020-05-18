#include "pch.h"
#include "d3d9.h"
#include "hook.h"
#include "hack.h"
#include "colors.h"

void D3D9::InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 0.f;
	style.WindowMinSize = ImVec2(IMGUI_WINDOW_W, IMGUI_WINDOW_H);
	style.WindowRounding = 0.f;
	style.TabRounding = 0.f;

	ImVec4* colors = style.Colors;
	colors[ImGuiCol_Text] = COLOR_ORANGE_TXT;
	colors[ImGuiCol_WindowBg] = COLOR_BLACK;
	colors[ImGuiCol_TitleBg] = COLOR_DARK_GRAY_3;
	colors[ImGuiCol_TitleBgActive] = COLOR_DARK_GRAY;
	colors[ImGuiCol_Tab] = COLOR_DARK_GRAY;
	colors[ImGuiCol_TabHovered] = COLOR_GRAY;
	colors[ImGuiCol_TabActive] = COLOR_DARK_GRAY_2;
	colors[ImGuiCol_CheckMark] = COLOR_ORANGE;
	colors[ImGuiCol_FrameBg] = COLOR_DARK_GRAY;
	colors[ImGuiCol_FrameBgHovered] = COLOR_DARK_GRAY_2;
	colors[ImGuiCol_FrameBgActive] = COLOR_GRAY;
	colors[ImGuiCol_SliderGrab] = COLOR_ORANGE;
	colors[ImGuiCol_SliderGrabActive] = COLOR_LIGHT_ORANGE;
	colors[ImGuiCol_Button] = COLOR_DARK_GRAY;
	colors[ImGuiCol_ButtonHovered] = COLOR_DARK_GRAY_2;
	colors[ImGuiCol_ButtonActive] = COLOR_GRAY;
	colors[ImGuiCol_TabHovered] = COLOR_DARK_GRAY;
	colors[ImGuiCol_Header] = COLOR_DARK_GRAY_2;
	colors[ImGuiCol_HeaderHovered] = COLOR_GRAY;
	colors[ImGuiCol_HeaderActive] = COLOR_GRAY_3;
	colors[ImGuiCol_MenuBarBg] = COLOR_DARK_GRAY_3;
	colors[ImGuiCol_Border] = COLOR_ORANGE;
	colors[ImGuiCol_ResizeGrip] = COLOR_DARK_ORANGE;
	colors[ImGuiCol_ResizeGripHovered] = COLOR_ORANGE;
	colors[ImGuiCol_ResizeGripActive] = COLOR_LIGHT_ORANGE;
	ImFontConfig cfg;
	cfg.FontDataOwnedByAtlas = false;
	Hack::Data::imgui_font = io.Fonts->AddFontFromMemoryTTF((void*)roboto_regular, sizeof(roboto_regular), IMGUI_FONT_SIZE, &cfg);
	ImGui_ImplWin32_Init(D3D9::hWnd);
	ImGui_ImplDX9_Init(pDevice);
}

bool init = false;
long __stdcall D3D9::hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if (!init)
	{
		InitImGui(pDevice);
		init = true;
	}

	Hack::Run(pDevice);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if(Hack::Data::bShowMenu)
		Hack::DrawMenu();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return oEndScene(pDevice); //Restore original EndScene function
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall D3D9::hkWndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KEYDOWN && wParam == KEY_MENU)
		Hack::Data::bShowMenu = !Hack::Data::bShowMenu;

	if (uMsg == WM_KEYDOWN || uMsg == WM_KEYUP)
		Hack::Data::KeyHook[wParam] = uMsg;

	if (Hack::Data::bShowMenu && uMsg & WM_ACTIVATE | WM_SHOWWINDOW | WM_CLOSE | WM_DESTROY)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Hooks::Init, hModule, 0, nullptr);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		Hooks::Shutdown();
		break;
	}
	return TRUE;
}