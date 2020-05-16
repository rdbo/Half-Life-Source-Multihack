#include "pch.h"
#include "d3d9.h"

namespace D3D9
{
	//Variables
	mem_t EndSceneAddress;
	EndScene_t oEndScene;
	WndProc_t oWndProc;
	HWND hWnd;
	void* vTable[119];
	ID3DXLine* dxLine;
	ID3DXFont* dxFont;
}

int Window::GetWidth()
{
	return this->right - this->left;
}

int Window::GetHeight()
{
	return this->bottom - this->top;
}

D3DCOLOR DrawColor::GetColorD3D()
{
	return D3DCOLOR_RGBA(this->r, this->g, this->b, this->a);
}

HWND g_hWnd;

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	pid_t wndPID;
	GetWindowThreadProcessId(handle, &wndPID);

	if (GetCurrentProcessId() != wndPID)
		return TRUE;

	g_hWnd = handle;
	return FALSE;
}

bool D3D9::CreateDummyDevice(void** vtable, size_t size)
{
	if (!vtable)
		return false;

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!pD3D)
		return false;

	IDirect3DDevice9* pDummyDevice = NULL;
	hWnd = Memory::In::GetCurrentWindowHandle();

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = false;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;

	HRESULT dummyDeviceCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

	if (dummyDeviceCreated != S_OK)
	{
		d3dpp.Windowed = !d3dpp.Windowed;

		dummyDeviceCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

		if (dummyDeviceCreated != S_OK)
		{
			pD3D->Release();
			return false;
		}
	}

	memcpy(vtable, *reinterpret_cast<void***>(pDummyDevice), size);

#	if RELEASE_D3D
	pDummyDevice->Release();
	pD3D->Release();
#	endif
	return true;
}

void D3D9::DrawLine(iVec2 src, iVec2 dst, int thickness, D3DCOLOR color, LPDIRECT3DDEVICE9 pDevice)
{
	if (!dxLine)
		D3DXCreateLine(pDevice, &dxLine);

	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(src.x, src.y);
	Line[1] = D3DXVECTOR2(dst.x, dst.y);
	dxLine->SetWidth(thickness);
	dxLine->Draw(Line, 2, color);
}

void D3D9::DrawString(LPCSTR text, int x, int y, D3DCOLOR color, LPD3DXFONT pFont)
{
	RECT rect;
	pFont->DrawTextA(NULL, text, strlen(text), &rect, DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));
	int size = rect.right -= rect.left;
	rect.left = x - size / 2;
	rect.right = x + size / 2;
	rect.top = y - size / 4;
	rect.bottom = y + ((size / 4) * 3);
	pFont->DrawTextA(NULL, text, -1, &rect, DT_NOCLIP, color);
}

bool D3D9::WorldToScreen(flVec3 pos, iVec2& screen, float viewMatrix[4][4], int windowWidth, int windowHeight)
{
	flVec4 clipCoords;
	clipCoords.x = pos.x * viewMatrix[0][0] + pos.y * viewMatrix[0][1] + pos.z * viewMatrix[0][2] + viewMatrix[0][3];
	clipCoords.y = pos.x * viewMatrix[1][0] + pos.y * viewMatrix[1][1] + pos.z * viewMatrix[1][2] + viewMatrix[1][3];
	clipCoords.z = pos.x * viewMatrix[2][0] + pos.y * viewMatrix[2][1] + pos.z * viewMatrix[2][2] + viewMatrix[2][3];
	clipCoords.w = pos.x * viewMatrix[3][0] + pos.y * viewMatrix[3][1] + pos.z * viewMatrix[3][2] + viewMatrix[3][3];

	if (clipCoords.w < 0.1f)
		return false;

	flVec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}

float D3D9::Get3DDistance(flVec3 src, flVec3 dst)
{
	return (float)sqrt
	(
		pow(dst.x - src.x, 2) +
		pow(dst.y - src.y, 2) +
		pow(dst.z - src.z, 2)
	);
}