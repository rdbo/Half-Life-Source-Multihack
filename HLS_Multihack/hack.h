#pragma once
#include "pch.h"
#include "d3d9.h"

#define KEY_MENU VK_INSERT
#define KEY_BHOP VK_SPACE

#define CLIENT_MODULE AUTO_STR("client.dll")
#define ENGINE_MODULE AUTO_STR("engine.dll")
#define SERVER_MODULE AUTO_STR("server.dll")
#define FONT_NAME AUTO_STR("Consolas")
#define CFONT_NAME "Consolas"
#define FONT_SIZE 14
#define MAX_ENTITIES 1024
#define BLACKLISTED_ENTITIES (char*)"monster_cockroach"
#define LOCALPLAYER_TYPE_ID 5
#define DECREASE_HEALTH_HOOK_LENGTH 8
#define DECREASE_AMMO_HOOK_LENGTH 5
#define DECREASE_AMMO_SHOTGUN_HOOK_LENGTH 5
#define DECREASE_AMMO_OTHERS_HOOK_LENGTH 5
#define RELOAD_INCREASE_CLIP_AMMO_HOOK_LENGTH 5
#define RELOAD_INCREASE_CLIP_AMMO_SHOTGUN_HOOK_LENGTH 6
#define RELOAD_DECREASE_AMMO_HOOK_LENGTH 6
#define MIN_ESP_RANGE 100
#define MAX_ESP_RANGE 2500
#define MIN_FOV 1
#define MAX_FOV 90
#define MIN_TIMESCALE 1
#define MAX_TIMESCALE 100
#define DEFAULT_FOV 90
#define MIN_GRAVITY 0
#define MAX_GRAVITY 10000
#define DEFAULT_GRAVITY 800
#define DEFAULT_TIMESCALE 1
#define MIN_POS -10000
#define MAX_POS 10000
#define MAX_VALUE 0x64
#define NOCLIP_STATE_OFF 131072
#define NOCLIP_STATE_ON 524288

namespace HLS
{
	namespace Offsets
	{
		namespace Client
		{
			const mem_t dwClientData = 0x432D8C;
			const mem_t dwJump = 0x4602C4;
			const mem_t dwAttack = 0x4602D0;
			const mem_t bCheckPlayerState = 0x458500;
			const mem_t bEnableCrosshair = 0x45D990;

			namespace ClientData
			{
				const mem_t dwFlags = 0x34C;
				const mem_t dwFieldOfView = 0xFD0;
				const mem_t bOnTarget = 0xFD8;
			}
		}

		namespace Engine
		{
			const mem_t dwViewMatrix = 0x58B4F0;
			const mem_t dwServerCheats = 0x5F17E8;
			const mem_t bLoading = 0x58BFFC;
			const mem_t flTimescale = 0x611F84;
		}

		namespace Server
		{
			const mem_t dwEntityList = 0x4B0FB4;
			const mem_t dwLocalPlayer = 0x4B0FC4;
			const mem_t fDecreaseHealth = 0xCC639;
			const mem_t fDecreaseAmmo = 0x2BA639;
			const mem_t fDecreaseAmmoShotgun = 0xC82EA;
			const mem_t fDecreaseAmmoOthers = 0xD0EC4;
			const mem_t fReloadIncreaseClipAmmo = 0xD383A;
			const mem_t fReloadIncreaseClipAmmoShotgun = 0x2C6D18;
			const mem_t fReloadDecreaseAmmo = 0xD0EC4;
			const mem_t flGravity = 0x4CA80C;

			namespace Entity
			{
				const mem_t strEntityType = 0x5C;
				const mem_t dwHealth = 0xE0;
				const mem_t dwEntityTypeId = 0x1DC;
				const mem_t vflPosition = 0x304;
				const mem_t dwArmor = 0xD00;
				const mem_t bDormant = 0x900;
				const mem_t dwNoClipState = 0x138;

				const mem_t dwLoopDistance = 0x10;
			}
		}
	}
}

class HLEntity
{
public:
	union
	{
		CREATE_UNION_MEMBER(char*, EntityTypeStr, HLS::Offsets::Server::Entity::strEntityType);
		CREATE_UNION_MEMBER(int, Health, HLS::Offsets::Server::Entity::dwHealth);
		CREATE_UNION_MEMBER(DWORD, TypeId, HLS::Offsets::Server::Entity::dwEntityTypeId);
		CREATE_UNION_MEMBER(int, Armor, HLS::Offsets::Server::Entity::dwArmor);
		CREATE_UNION_MEMBER(flVec3, Position, HLS::Offsets::Server::Entity::vflPosition);
		CREATE_UNION_MEMBER(bool, Dormant, HLS::Offsets::Server::Entity::bDormant);
		CREATE_UNION_MEMBER(DWORD, NoClipState, HLS::Offsets::Server::Entity::dwNoClipState);
	};
};

class HLClientData
{
public:
	union
	{
		CREATE_UNION_MEMBER(DWORD, Flags, HLS::Offsets::Client::ClientData::dwFlags);
		CREATE_UNION_MEMBER(int, FieldOfView, HLS::Offsets::Client::ClientData::dwFieldOfView);
		CREATE_UNION_MEMBER(bool, OnTarget, HLS::Offsets::Client::ClientData::bOnTarget);
	};
};

class Crosshair
{
public:
	iVec2 Position;
	DrawColor Color;
	int Thickness;
	int Gap;
	int Size;
};

typedef struct { float Matrix[4][4]; } ViewMatrix;

namespace Hack
{
	void Run(LPDIRECT3DDEVICE9 pDevice);
	void DrawMenu();
	void hkDecreaseHealth();
	void hkDecreaseAmmo();
	void hkDecreaseAmmoShotgun();
	void hkDecreaseAmmoOthers();
	void hkReloadIncreaseClipAmmo();
	void hkReloadIncreaseClipAmmoShotgun();
	void hkReloadDecreaseAmmo();

	void ESP_Snaplines(HLEntity* ent, iVec2 Ent2DPos, LPDIRECT3DDEVICE9 pDevice, Window wnd);
	void ESP_Name(HLEntity* ent, iVec2 Ent2DPos, LPDIRECT3DDEVICE9 pDevice);
	bool FilterEntity(HLEntity* ent);
	void HookDecreaseHealth();
	void HookDecreaseAmmo();
	void ServerCheatsChecker();
	void Bunnyhop();
	void Triggerbot();
	void NoClip();
	void Teleport();
	void InfiniteHealth();
	void InfiniteArmor();
	void FovChanger();
	void GravityChanger();
	void TimescaleChanger();
	void DrawCrosshair(Crosshair xhair, LPDIRECT3DDEVICE9 pDevice);
	void ServerCheats();

	namespace Data
	{
		//Menu
		extern bool bShowMenu;
		extern bool bBunnyhop;
		extern bool bTriggerbot;
		extern bool bEspSnaplines;
		extern bool bEspLimitedRange;
		extern bool bEspName;
		extern bool bEnableEntityFilter;
		extern bool bGodmode;
		extern bool bOneHitKills;
		extern bool bInfiniteHealth;
		extern bool bInfiniteArmor;
		extern bool bNoAmmoDecrease;
		extern bool bNoClip;
		extern float flEspRange;
		extern bool bCustomCrosshair;
		extern bool bShowNameColor;
		extern bool bShowSnaplineColor;
		extern bool bShowCrosshairSettings;
		extern bool bFovChanger;
		extern int iNewFov;
		extern bool bTeleport;
		extern bool bTeleportPending;
		extern bool bServerCheats;
		extern bool bGravityChanger;
		extern float flNewGravity;
		extern bool bTimescaleChanger;
		extern float flNewTimescale;
		extern flVec3 vTeleportPos;
		extern DrawColor SnaplineColor;
		extern DrawColor NameColor;
		extern Crosshair crosshair;
		extern Window wnd;
		extern ImFont* imgui_font;

		//Game
		extern mem_t client;
		extern mem_t engine;
		extern mem_t server;
		extern mem_t fDecreaseHealthAddr;
		extern mem_t fDecreaseAmmoAddr;
		extern mem_t fDecreaseAmmoShotgunAddr;
		extern mem_t fDecreaseAmmoOthersAddr;
		extern mem_t fReloadIncreaseClipAmmoAddr;
		extern mem_t fReloadIncreaseClipAmmoShotgunAddr;
		extern mem_t fReloadDecreaseAmmoAddr;
		extern HLEntity* LocalPlayer;
		extern HLEntity* Entity;
		extern bool* CheckPlayerState;
		extern bool* Loading;
		extern mem_t EntityList;
		extern std::vector<char*> EntityStringFilter;
		extern UINT KeyHook[0xFE];
		extern ViewMatrix vMatrix;
		extern HLClientData* ClientData;
		extern DWORD* ForceJump;
		extern DWORD* ForceAttack;
		extern DWORD* EnableCrosshair;
		extern DWORD* SvCheats;
		extern float* Gravity;
		extern float* Timescale;
	}
}