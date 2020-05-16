#pragma once
#include "pch.h"
#include "d3d9.h"

#if defined(UCS)
#define AUTO_STR(str) L##str
#elif defined(MBCS)
#define AUTO_STR(str) str
#endif

#define KEY_MENU VK_INSERT
#define KEY_BHOP VK_SPACE

#define CLIENT_MODULE AUTO_STR("client.dll")
#define ENGINE_MODULE AUTO_STR("engine.dll")
#define SERVER_MODULE AUTO_STR("server.dll")
#define FONT_NAME AUTO_STR("Consolas")
#define FONT_SIZE 14
#define MAX_ENTITIES 1024
#define BLACKLISTED_ENTITIES (char*)"monster_cockroach"
#define LOCALPLAYER_TYPE_ID 5
#define DECREASE_HEALTH_HOOK_LENGTH 8
#define MAX_VALUE 0x100

namespace HLS
{
	namespace Offsets
	{
		namespace Client
		{
			const mem_t dwClientData = 0x432D8C;
			const mem_t dwJump = 0x4602C4;
			const mem_t bCheckPlayerState = 0x458500;
			const mem_t bEnableCrosshair = 0x45D990;

			namespace ClientData
			{
				const mem_t dwFlags = 0x34C;
			}
		}

		namespace Engine
		{
			const mem_t dwViewMatrix = 0x58B4F0;
		}

		namespace Server
		{
			const mem_t dwEntityList = 0x4B0FB4;
			const mem_t dwLocalPlayer = 0x4B0FC4;
			const mem_t fDecreaseHealth = 0xCC639;

			namespace Entity
			{
				const mem_t strEntityType = 0x5C;
				const mem_t dwHealth = 0xE0;
				const mem_t dwEntityTypeId = 0x1DC;
				const mem_t vflPosition = 0x304;
				const mem_t dwArmor = 0xD00;
				const mem_t bDormant = 0x900;

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
	};
};

class HLClientData
{
public:
	union
	{
		CREATE_UNION_MEMBER(DWORD, Flags, HLS::Offsets::Client::ClientData::dwFlags);
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

	void ESP_Snaplines(HLEntity* ent, iVec2 Ent2DPos, LPDIRECT3DDEVICE9 pDevice, Window wnd);
	void ESP_Name(HLEntity* ent, iVec2 Ent2DPos, LPDIRECT3DDEVICE9 pDevice);
	bool FilterEntity(HLEntity* ent);
	void HookDecreaseHealth();
	void Bunnyhop();
	void InfiniteHealth();
	void InfiniteArmor();
	void DrawCrosshair(Crosshair xhair, LPDIRECT3DDEVICE9 pDevice);

	namespace Data
	{
		//Menu
		extern bool bShowMenu;
		extern bool bBunnyhop;
		extern bool bEspSnaplines;
		extern bool bEspLimitedRange;
		extern bool bEspName;
		extern bool bEnableEntityFilter;
		extern bool bGodmode;
		extern bool bOneHitKills;
		extern bool bInfiniteHealth;
		extern bool bInfiniteArmor;
		extern float flEspRange;
		extern bool bCustomCrosshair;
		extern DrawColor SnaplineColor;
		extern DrawColor NameColor;
		extern Crosshair crosshair;
		extern Window wnd;

		//Game
		extern mem_t client;
		extern mem_t engine;
		extern mem_t server;
		extern mem_t fDecreaseHealthAddr;
		extern HLEntity* LocalPlayer;
		extern HLEntity* Entity;
		extern bool* CheckPlayerState;
		extern mem_t EntityList;
		extern std::vector<char*> EntityStringFilter;
		extern UINT KeyHook[0xFE];
		extern ViewMatrix vMatrix;
		extern HLClientData* ClientData;
		extern DWORD* ForceJump;
		extern DWORD* EnableCrosshair;
	}
}