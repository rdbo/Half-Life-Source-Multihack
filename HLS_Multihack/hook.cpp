#include "pch.h"
#include "hook.h"
#include "d3d9.h"

void Hooks::Init()
{
	if (!D3D9::CreateDummyDevice(D3D9::vTable, sizeof(D3D9::vTable))) return;
	D3D9::EndSceneAddress = (mem_t)D3D9::vTable[42];
	D3D9::oEndScene = (D3D9::EndScene_t)Memory::In::Hook::TrampolineHook((byte_t*)D3D9::EndSceneAddress, (byte_t*)D3D9::hkEndScene, ENDSCENE_HOOK_LENGTH);
	D3D9::oWndProc = (WNDPROC)SetWindowLongPtr(D3D9::hWnd, WNDPROC_INDEX, (LONG_PTR)D3D9::hkWndProc);
}

void Hooks::Shutdown()
{
	if (!D3D9::EndSceneAddress) return;
	Memory::In::Hook::Restore(D3D9::EndSceneAddress);
}