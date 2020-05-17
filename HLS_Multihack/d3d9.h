#pragma once
#include "pch.h"
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#if defined(ARCH_X86)
#define ENDSCENE_HOOK_LENGTH 7
#define WNDPROC_INDEX GWL_WNDPROC
#elif defined(ARCH_X64)
#define ENDSCENE_HOOK_LENGTH 15
#define WNDPROC_INDEX GWLP_WNDPROC
#endif

#define IMGUI_WINDOW_W 500
#define IMGUI_WINDOW_H 350
#define IMGUI_FONT_SIZE 18

#define RELEASE_D3D 1

struct iVec2
{
	int x, y;
};

struct flVec2
{
	float x, y;
};

struct flVec3
{
	float x, y, z;
};

struct flVec4
{
	float x, y, z, w;
};

struct Window : RECT
{
	int GetWidth();
	int GetHeight();
};

struct DrawColor
{
	int r, g, b, a;
	D3DCOLOR GetColorD3D();
};

namespace D3D9
{
	typedef long(__stdcall* EndScene_t)(LPDIRECT3DDEVICE9);
	typedef LRESULT(__stdcall* WndProc_t)(const HWND, UINT, WPARAM, LPARAM);
	extern mem_t EndSceneAddress;
	extern EndScene_t oEndScene;
	extern WndProc_t oWndProc;
	extern HWND hWnd;
	extern void* vTable[119];
	extern ID3DXLine* dxLine;
	extern ID3DXFont* dxFont;

	void InitImGui(LPDIRECT3DDEVICE9 pDevice);
	LRESULT __stdcall hkWndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool CreateDummyDevice(void** vtable, size_t size);
	long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice);
	void DrawLine(iVec2 src, iVec2 dst, int thickness, D3DCOLOR color, LPDIRECT3DDEVICE9 pDevice);
	void DrawString(LPCSTR text, int x, int y, D3DCOLOR color, LPD3DXFONT pFont);
	bool WorldToScreen(flVec3 pos, iVec2& screen, float viewMatrix[4][4], int windowWidth, int windowHeight);
	float Get3DDistance(flVec3 src, flVec3 dst);
}