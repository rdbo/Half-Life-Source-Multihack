#include "pch.h"
#include "hack.h"

namespace Hack
{
	namespace Data
	{
		//Menu
		bool bInitialized = false;
		bool bShowMenu = false;
		bool bBunnyhop = false;
		bool bEspSnaplines = false;
		bool bEspName = false;
		bool bEspLimitedRange = false;
		bool bEnableEntityFilter = false;
		bool bGodmode = false;
		bool bOneHitKills = false;
		bool bInfiniteHealth = false;
		bool bInfiniteArmor = false;
		bool bNoAmmoDecrease = false;
		bool bCustomCrosshair = false;
		bool bShowNameColor = false;
		bool bShowSnaplineColor = false;
		bool bShowCrosshairSettings = false;
		float flEspRange = 750;
		DrawColor SnaplineColor = { 255, 0, 0, 255 };
		DrawColor NameColor = { 255, 125, 0, 255 };
		Crosshair crosshair = { { 0, 0 }, { 125, 255, 0, 255 }, 1, 2, 10 };
		Window wnd;
		ImFont* imgui_font;

		//Game Data
		mem_t client;
		mem_t engine;
		mem_t server;
		mem_t fDecreaseHealthAddr;
		mem_t fDecreaseAmmoAddr;
		mem_t fDecreaseAmmoShotgunAddr;
		mem_t fDecreaseAmmoOthersAddr;
		bool* CheckPlayerState;
		HLEntity* LocalPlayer;
		HLEntity* Entity;
		mem_t EntityList;
		std::vector<char*> EntityStringFilter = { BLACKLISTED_ENTITIES };
		UINT KeyHook[0xFE];
		ViewMatrix vMatrix;
		HLClientData* ClientData;
		DWORD* ForceJump;
		DWORD* EnableCrosshair;
	}
}

mem_t DecHealthJumpAddr;
mem_t DecAmmoJumpAddr;
mem_t DecAmmoShotgunJumpAddr;
mem_t DecAmmoOthersJumpAddr;

void Hack::DrawMenu()
{
	/*
	ImGui::Begin("HL:S Multihack by rdbo");
	ImGui::Checkbox("Bunnyhop", &Data::bBunnyhop);
	ImGui::Checkbox("ESP Snaplines", &Data::bEspSnaplines);
	ImGui::Checkbox("ESP Name", &Data::bEspName);
	ImGui::Checkbox("ESP Limited Range", &Data::bEspLimitedRange);
	ImGui::Checkbox("Infinite Health", &Data::bInfiniteHealth);
	ImGui::Checkbox("Infinite Armor", &Data::bInfiniteArmor);
	ImGui::Checkbox("Custom Crosshair", &Data::bCustomCrosshair);
	ImGui::SliderInt("Crosshair Thickness", &Data::crosshair.Thickness, 0, Data::wnd.GetWidth());
	ImGui::SliderInt("Crosshair Size", &Data::crosshair.Size, 0, Data::wnd.GetWidth());
	ImGui::SliderInt("Crosshair Gap", &Data::crosshair.Gap, 0, Data::wnd.GetWidth());
	ImGui::SliderInt("Crosshair R", &Data::crosshair.Color.r, 0, 255);
	ImGui::SliderInt("Crosshair G", &Data::crosshair.Color.g, 0, 255);
	ImGui::SliderInt("Crosshair B", &Data::crosshair.Color.b, 0, 255);
	ImGui::SliderInt("Crosshair A", &Data::crosshair.Color.a, 0, 255);
	if (ImGui::Checkbox("Godmode", &Data::bGodmode))
	{
		Hack::HookDecreaseHealth();
	}

	if (ImGui::Checkbox("One Hit Kills", &Data::bOneHitKills))
	{
		Hack::HookDecreaseHealth();
	}
	if (ImGui::Checkbox("No Ammo Decrease", &Data::bNoAmmoDecrease))
	{
		Hack::HookDecreaseAmmo();
	}
	ImGui::Checkbox("Enable Entity Filter", &Data::bEnableEntityFilter);
	ImGui::SliderFloat("ESP Range: ", &Data::flEspRange, MIN_ESP_RANGE, MAX_ESP_RANGE, "%1.f");
	ImGui::Separator();
	ImGui::SliderInt("ESP Snapline R", &Data::SnaplineColor.r, 0, 255);
	ImGui::SliderInt("ESP Snapline G", &Data::SnaplineColor.g, 0, 255);
	ImGui::SliderInt("ESP Snapline B", &Data::SnaplineColor.b, 0, 255);
	ImGui::SliderInt("ESP Snapline A", &Data::SnaplineColor.a, 0, 255);
	ImGui::Separator();
	ImGui::SliderInt("ESP Name R", &Data::NameColor.r, 0, 255);
	ImGui::SliderInt("ESP Name G", &Data::NameColor.g, 0, 255);
	ImGui::SliderInt("ESP Name B", &Data::NameColor.b, 0, 255);
	ImGui::SliderInt("ESP Name A", &Data::NameColor.a, 0, 255);
	ImGui::End();
	*/

	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("ESP"))
	{
		ImGui::Checkbox("ESP Snaplines", &Data::bEspSnaplines);
		ImGui::Checkbox("ESP Name", &Data::bEspName);
		ImGui::Checkbox("Enable Entity Filter", &Data::bEnableEntityFilter);
		ImGui::Checkbox("ESP Limited Range", &Data::bEspLimitedRange);
		if (Data::bEspLimitedRange)
		{
			ImGui::SliderFloat("ESP Range", &Data::flEspRange, MIN_ESP_RANGE, MAX_ESP_RANGE, "%1.f");
		}

		ImGui::Checkbox("Show Snapline Color Properties", &Data::bShowSnaplineColor);
		if (Data::bShowSnaplineColor)
		{
			ImGui::SliderInt("ESP Snapline R", &Data::SnaplineColor.r, 0, 255);
			ImGui::SliderInt("ESP Snapline G", &Data::SnaplineColor.g, 0, 255);
			ImGui::SliderInt("ESP Snapline B", &Data::SnaplineColor.b, 0, 255);
			ImGui::SliderInt("ESP Snapline A", &Data::SnaplineColor.a, 0, 255);
		}

		ImGui::Checkbox("Show Name Color Properties", &Data::bShowNameColor);
		if (Data::bShowNameColor)
		{
			ImGui::SliderInt("ESP Name R", &Data::NameColor.r, 0, 255);
			ImGui::SliderInt("ESP Name G", &Data::NameColor.g, 0, 255);
			ImGui::SliderInt("ESP Name B", &Data::NameColor.b, 0, 255);
			ImGui::SliderInt("ESP Name A", &Data::NameColor.a, 0, 255);
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("CROSSHAIR"))
	{
		ImGui::Checkbox("Enable Custom Crosshair", &Data::bCustomCrosshair);
		ImGui::Checkbox("Show Crosshair Properties", &Data::bShowCrosshairSettings);
		if (Data::bShowCrosshairSettings)
		{
			ImGui::SliderInt("Crosshair Thickness", &Data::crosshair.Thickness, 0, Data::wnd.GetWidth());
			ImGui::SliderInt("Crosshair Size", &Data::crosshair.Size, 0, Data::wnd.GetWidth());
			ImGui::SliderInt("Crosshair Gap", &Data::crosshair.Gap, 0, Data::wnd.GetWidth());
			ImGui::SliderInt("Crosshair R", &Data::crosshair.Color.r, 0, 255);
			ImGui::SliderInt("Crosshair G", &Data::crosshair.Color.g, 0, 255);
			ImGui::SliderInt("Crosshair B", &Data::crosshair.Color.b, 0, 255);
			ImGui::SliderInt("Crosshair A", &Data::crosshair.Color.a, 0, 255);
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("CHEATS"))
	{
		ImGui::Checkbox("Bunnyhop", &Data::bBunnyhop);
		ImGui::Checkbox("Infinite Health", &Data::bInfiniteHealth);
		ImGui::Checkbox("Infinite Armor", &Data::bInfiniteArmor);
		if (ImGui::Checkbox("Godmode", &Data::bGodmode))
		{
			Hack::HookDecreaseHealth();
		}

		if (ImGui::Checkbox("One Hit Kills", &Data::bOneHitKills))
		{
			Hack::HookDecreaseHealth();
		}
		if (ImGui::Checkbox("No Ammo Decrease", &Data::bNoAmmoDecrease))
		{
			Hack::HookDecreaseAmmo();
		}

		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}

void Hack::Run(LPDIRECT3DDEVICE9 pDevice)
{
	if (!Data::bInitialized)
	{
		Data::client = Memory::In::GetModuleAddress(CLIENT_MODULE);
		Data::engine = Memory::In::GetModuleAddress(ENGINE_MODULE);
		Data::server = Memory::In::GetModuleAddress(SERVER_MODULE);
		if (!Data::client || !Data::engine || !Data::server) return;
		Data::ForceJump = (DWORD*)(Data::client + HLS::Offsets::Client::dwJump);
		Data::CheckPlayerState = (bool*)(Data::client + HLS::Offsets::Client::bCheckPlayerState);
		Data::EnableCrosshair = (DWORD*)(Data::client + HLS::Offsets::Client::bEnableCrosshair);
		Data::fDecreaseHealthAddr = (Data::server + HLS::Offsets::Server::fDecreaseHealth);
		Data::fDecreaseAmmoAddr = (Data::server + HLS::Offsets::Server::fDecreaseAmmo);
		Data::fDecreaseAmmoShotgunAddr = (Data::server + HLS::Offsets::Server::fDecreaseAmmoShotgun);
		Data::fDecreaseAmmoOthersAddr = (Data::server + HLS::Offsets::Server::fDecreaseAmmoOthers);
		DecHealthJumpAddr = Data::fDecreaseHealthAddr + DECREASE_HEALTH_HOOK_LENGTH;
		DecAmmoJumpAddr = Data::fDecreaseAmmoAddr + DECREASE_AMMO_HOOK_LENGTH;
		DecAmmoShotgunJumpAddr = Data::fDecreaseAmmoShotgunAddr + DECREASE_AMMO_SHOTGUN_HOOK_LENGTH;
		DecAmmoOthersJumpAddr = Data::fDecreaseAmmoOthersAddr + DECREASE_AMMO_OTHERS_HOOK_LENGTH;
		D3DXCreateFont(pDevice, FONT_SIZE, 0, FW_REGULAR, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, FONT_NAME, &D3D9::dxFont);
		Data::bInitialized = true;
	}

	GetClientRect(D3D9::hWnd, &Data::wnd);
	Data::crosshair.Position.x = Data::wnd.left + Data::wnd.GetWidth() / 2;
	Data::crosshair.Position.y = Data::wnd.top + Data::wnd.GetHeight() / 2;

	if (!*Data::CheckPlayerState) return;
	Data::LocalPlayer = *(HLEntity**)(Data::server + HLS::Offsets::Server::dwLocalPlayer);
	if (!Data::LocalPlayer) return;

	InfiniteHealth();
	InfiniteArmor();
	Bunnyhop();
	DrawCrosshair(Data::crosshair, pDevice);

	Data::EntityList = (mem_t)(Data::server + HLS::Offsets::Server::dwEntityList);
	Data::vMatrix = *(ViewMatrix*)(Data::engine + HLS::Offsets::Engine::dwViewMatrix);
	Data::ClientData = *(HLClientData**)(Data::client + HLS::Offsets::Client::dwClientData);

	for (size_t i = 0; i < MAX_ENTITIES; i++)
	{
		if (Data::LocalPlayer->Health <= 0 || !&Data::CheckPlayerState) break;
		Data::Entity = *(HLEntity**)(Data::EntityList + i * HLS::Offsets::Server::Entity::dwLoopDistance);
		if (*Data::CheckPlayerState && Data::Entity && (mem_t)Data::Entity != (mem_t)Data::LocalPlayer && !Data::Entity->Dormant && Data::Entity->Health > 0 && Data::Entity->TypeId != 0 && FilterEntity(Data::Entity))
		{
			iVec2 Ent2DPos;
			if (D3D9::WorldToScreen(Data::Entity->Position, Ent2DPos, Data::vMatrix.Matrix, Data::wnd.GetWidth(), Data::wnd.GetHeight()))
			{
				if (!Data::bEspLimitedRange || (Data::bEspLimitedRange && Data::flEspRange >= D3D9::Get3DDistance(Data::LocalPlayer->Position, Data::Entity->Position)))
				{
					ESP_Snaplines(Data::Entity, Ent2DPos, pDevice, Data::wnd);
					ESP_Name(Data::Entity, Ent2DPos, pDevice);
				}
			}
		}
	}
	int a;
}

void Hack::DrawCrosshair(Crosshair xhair, LPDIRECT3DDEVICE9 pDevice)
{
	if (Data::bCustomCrosshair)
	{
		if (*Data::EnableCrosshair == 1)
			*Data::EnableCrosshair = 0;
		//Left
		D3D9::DrawLine({ xhair.Position.x - xhair.Gap - xhair.Size, xhair.Position.y }, { xhair.Position.x - xhair.Gap, xhair.Position.y }, xhair.Thickness, xhair.Color.GetColorD3D(), pDevice);
		//Right
		D3D9::DrawLine({ xhair.Position.x + xhair.Gap, xhair.Position.y }, { xhair.Position.x + xhair.Gap + xhair.Size, xhair.Position.y }, xhair.Thickness, xhair.Color.GetColorD3D(), pDevice);
		//Top
		D3D9::DrawLine({ xhair.Position.x, xhair.Position.y - xhair.Gap - xhair.Size }, { xhair.Position.x, xhair.Position.y - xhair.Gap }, xhair.Thickness, xhair.Color.GetColorD3D(), pDevice);
		//Bottom
		D3D9::DrawLine({ xhair.Position.x, xhair.Position.y + xhair.Gap }, { xhair.Position.x, xhair.Position.y + xhair.Gap + xhair.Size }, xhair.Thickness, xhair.Color.GetColorD3D(), pDevice);
	}

	else if(*Data::EnableCrosshair == 0)
		*Data::EnableCrosshair = 1;
}

void Hack::Bunnyhop()
{
	if (Data::bBunnyhop && Data::KeyHook[KEY_BHOP] == WM_KEYDOWN && Data::ClientData && Data::ClientData->Flags & (1 << 0))
	{
		*Data::ForceJump = 6;
	}
}

void Hack::InfiniteHealth()
{
	if (Data::bInfiniteHealth && *Data::CheckPlayerState && Data::LocalPlayer && Data::LocalPlayer->Health < MAX_VALUE)
	{
		Data::LocalPlayer->Health = MAX_VALUE;
	}
}

void Hack::InfiniteArmor()
{
	if (Data::bInfiniteArmor && *Data::CheckPlayerState && Data::LocalPlayer && Data::LocalPlayer->Armor < MAX_VALUE)
	{
		Data::LocalPlayer->Armor = MAX_VALUE;
	}
}

__declspec(naked)
void Hack::hkDecreaseHealth()
{
	__asm
	{
		push esi
		sub esi, HLS::Offsets::Server::Entity::dwHealth //Go to base address
		add esi, HLS::Offsets::Server::Entity::dwEntityTypeId //Get entity type
		cmp [esi], LOCALPLAYER_TYPE_ID //Check If the entity is the localplayer
		jne enemy
		jmp player

		enemy:
		pop esi
		cmp Data::bOneHitKills, 1
		jne originalcode
		mov edi, 0
		jmp originalcode

		player:
		pop esi
		cmp Data::bGodmode, 1
		jne originalcode
		mov edi, MAX_VALUE
		jmp originalcode

		originalcode: //Stolen bytes
		mov[esi], edi //Set Health
		pop edi
		mov eax, 00000001
		jmp DecHealthJumpAddr
	}
}

void Hack::HookDecreaseHealth()
{
	bool bEnable = Data::bGodmode || Data::bOneHitKills;
	if (bEnable && Data::fDecreaseHealthAddr)
		Memory::In::Hook::Detour((byte_t*)Data::fDecreaseHealthAddr, (byte_t*)Hack::hkDecreaseHealth, DECREASE_HEALTH_HOOK_LENGTH);
	else
		Memory::In::Hook::Restore(Data::fDecreaseHealthAddr);
}

void Hack::HookDecreaseAmmo()
{
	if (Data::bNoAmmoDecrease && Data::fDecreaseAmmoAddr && Data::fDecreaseAmmoShotgunAddr && Data::fDecreaseAmmoOthersAddr)
	{
		Memory::In::Hook::Detour((byte_t*)Data::fDecreaseAmmoAddr, (byte_t*)Hack::hkDecreaseAmmo, DECREASE_AMMO_HOOK_LENGTH);
		Memory::In::Hook::Detour((byte_t*)Data::fDecreaseAmmoShotgunAddr, (byte_t*)Hack::hkDecreaseAmmoShotgun, DECREASE_AMMO_SHOTGUN_HOOK_LENGTH);
		Memory::In::Hook::Detour((byte_t*)Data::fDecreaseAmmoOthersAddr, (byte_t*)Hack::hkDecreaseAmmoOthers, DECREASE_AMMO_OTHERS_HOOK_LENGTH);
	}
}

__declspec(naked)
void Hack::hkDecreaseAmmo()
{
	__asm
	{
		mov ebx, MAX_VALUE
		jmp originalcode

		originalcode:
		mov[esi], ebx
		mov eax, edi
		pop edi

		jmp DecAmmoJumpAddr
	}
}

__declspec(naked)
void Hack::hkDecreaseAmmoShotgun()
{
	__asm
	{
		mov edi, MAX_VALUE
		jmp originalcode

		originalcode:
		mov[esi], edi
		pop edi
		mov eax, esi

		exit:
		jmp DecAmmoShotgunJumpAddr
	}
}

__declspec(naked)
void Hack::hkDecreaseAmmoOthers()
{
	__asm
	{
		mov ebx, MAX_VALUE

		originalcode:
		mov[esi], ebx
		pop edi
		pop esi
		pop ebx

		exit:
		jmp DecAmmoOthersJumpAddr
	}
}

bool Hack::FilterEntity(HLEntity* ent)
{
	if (!Data::bEnableEntityFilter) return true;
	size_t size = Data::EntityStringFilter.size();
	bool check = true;
	for (size_t i = 0; i < size; i++)
	{
		char* blstr = Data::EntityStringFilter.at(i);
		check &= !!strcmp(blstr, ent->EntityTypeStr);

		if (!check) break;
	}

	return check;
}

void Hack::ESP_Snaplines(HLEntity* ent, iVec2 Ent2DPos, LPDIRECT3DDEVICE9 pDevice, Window wnd)
{
	if (Data::bEspSnaplines)
	{
		int wWidth = wnd.GetWidth();
		int wHeight = wnd.GetHeight();
		iVec2 screenCoords = { wWidth / 2, wHeight };

		D3D9::DrawLine(screenCoords, Ent2DPos, 2, Data::SnaplineColor.GetColorD3D(), pDevice);
	}
}

void Hack::ESP_Name(HLEntity* ent, iVec2 Ent2DPos, LPDIRECT3DDEVICE9 pDevice)
{
	if (Data::bEspName && D3D9::dxFont)
	{
		D3D9::DrawString(ent->EntityTypeStr, Ent2DPos.x, Ent2DPos.y, Data::NameColor.GetColorD3D(), D3D9::dxFont);
	}
}