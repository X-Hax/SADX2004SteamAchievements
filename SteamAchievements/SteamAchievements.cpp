// SteamAchievements.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "modloader\SADXModLoader.h"
#include "steam\steam_api.h"

bool callbackreceived = false;

class CallbackStuff
{
public:
	STEAM_CALLBACK(CallbackStuff, OnUserStatsReceived, UserStatsReceived_t, m_OnUserStatsReceived);

	CallbackStuff() : m_OnUserStatsReceived(this, &CallbackStuff::OnUserStatsReceived)
	{
	}
};

void CallbackStuff::OnUserStatsReceived(UserStatsReceived_t *pParam)
{
	callbackreceived = true;
}

CallbackStuff *callbackobj;

void SetAchievement(const char *id)
{
	if (callbackreceived)
	{
		SteamUserStats()->SetAchievement(id);
		SteamUserStats()->StoreStats();
	}
}

void UnlockCharacterAdventure_r(int character)
{
	switch (character)
	{
	case Characters_Sonic:
		*(uint8_t *)0x3B18849 = 1;
		break;
	case Characters_Tails:
		*(uint8_t *)0x3B1884A = 1;
		SetAchievement("NEW_ACHIEVEMENT_1_0"); // Miles "Tails" Prower
		break;
	case Characters_Knuckles:
		*(uint8_t *)0x3B1884B = 1;
		SetAchievement("NEW_ACHIEVEMENT_1_1"); // Knuckles the Echidna
		break;
	case Characters_Amy:
		*(uint8_t *)0x3B1884C = 1;
		SetAchievement("NEW_ACHIEVEMENT_1_2"); // Amy Rose
		break;
	case Characters_Gamma:
		*(uint8_t *)0x3B1884D = 1;
		SetAchievement("NEW_ACHIEVEMENT_1_3"); // E-102 "Gamma"
		break;
	case Characters_Big:
		*(uint8_t *)0x3B1884E = 1;
		SetAchievement("NEW_ACHIEVEMENT_1_4"); // Big the Cat
		break;
	}
}

void __cdecl sub_4130E0_r()
{
	if (*(uint16_t*)0x3B18A24 != Characters_Tikal)
	{
		switch (*(uint16_t*)0x3B18A24)
		{
		case Characters_Sonic:
			*(uint8_t*)0x3B18850 = 1;
			*(uint8_t*)0x3B188A9 = 1;
			*(uint8_t*)0x3B188AA = 1;
			*(uint8_t*)0x3B188AB = 1;
			*(uint8_t*)0x3B1885A = 1;
			*(uint8_t*)0x3B188BF = 1;
			*(uint8_t*)0x3B188C0 = 1;
			*(uint8_t*)0x3B188C1 = 1;
			SetAchievement("NEW_ACHIEVEMENT_1_5"); // Sonic the Hedgehog
			break;
		case Characters_Tails:
			*(uint8_t*)0x3B18851 = 1;
			*(uint8_t*)0x3B188E4 = 1;
			*(uint8_t*)0x3B188E5 = 1;
			*(uint8_t*)0x3B188E6 = 1;
			*(uint8_t*)0x3B1885B = 1;
			*(uint8_t*)0x3B188F6 = 1;
			*(uint8_t*)0x3B188F7 = 1;
			*(uint8_t*)0x3B188F8 = 1;
			break;
		case Characters_Knuckles:
			*(uint8_t*)0x3B18917 = 1;
			*(uint8_t*)0x3B18852 = 1;
			*(uint8_t*)0x3B18923 = 1;
			*(uint8_t*)0x3B18924 = 1;
			*(uint8_t*)0x3B18925 = 1;
			*(uint8_t*)0x3B1885C = 1;
			*(uint8_t*)0x3B1892F = 1;
			*(uint8_t*)0x3B18930 = 1;
			*(uint8_t*)0x3B18931 = 1;
			break;
		case Characters_Amy:
			*(uint8_t*)0x3B18853 = 1;
			*(uint8_t*)0x3B1895B = 1;
			*(uint8_t*)0x3B1895C = 1;
			*(uint8_t*)0x3B1895D = 1;
			*(uint8_t*)0x3B1885D = 1;
			*(uint8_t*)0x3B18963 = 1;
			*(uint8_t*)0x3B18964 = 1;
			*(uint8_t*)0x3B18965 = 1;
			break;
		case Characters_Big:
			*(uint8_t*)0x3B189D3 = 1;
			*(uint8_t*)0x3B18855 = 1;
			*(uint8_t*)0x3B189DA = 1;
			*(uint8_t*)0x3B189DB = 1;
			*(uint8_t*)0x3B189DC = 1;
			*(uint8_t*)0x3B1885F = 1;
			*(uint8_t*)0x3B189E2 = 1;
			*(uint8_t*)0x3B189E3 = 1;
			*(uint8_t*)0x3B189E4 = 1;
			break;
		case Characters_Gamma:
			*(uint8_t*)0x3B18854 = 1;
			*(uint8_t*)0x3B1899F = 1;
			*(uint8_t*)0x3B189A0 = 1;
			*(uint8_t*)0x3B189A1 = 1;
			*(uint8_t*)0x3B1885E = 1;
			*(uint8_t*)0x3B189AA = 1;
			*(uint8_t*)0x3B189AB = 1;
			*(uint8_t*)0x3B189AC = 1;
			break;
		}
		if (!*(uint8_t*)0x3B1884F && *(uint8_t*)0x3B18850 && *(uint8_t*)0x3B18851 && *(uint8_t*)0x3B18852 && *(uint8_t*)0x3B18853 && *(uint8_t*)0x3B18855)
			if (*(uint8_t*)0x3B18854)
			{
				*(uint8_t*)0x3B1884F = 1;
				SetAchievement("NEW_ACHIEVEMENT_1_6"); // Super Sonic
			}
	}
}

VoidFunc(SaveSave, 0x42D630);
void SetEmblemCollected_r(SaveFileData *savefile, signed int index)
{
	savefile->Emblems[index / 8] |= 1 << index % 8;
	if (GameMode == GameModes_Adventure_Field || GameMode == GameModes_Mission)
	{
		if (*(int32_t*)0x3B29198)
			SaveSave();
		else
			*(uint8_t*)0x3B291AD = 1;
	}
	if (callbackreceived)
	{
		int count = CountEmblems(savefile);
		if (count == 130)
		{
			SetAchievement("NEW_ACHIEVEMENT_1_11"); // The Perfect Adventurer
			SetAchievement("NEW_ACHIEVEMENT_1_12"); // Metal Sonic
		}
		else if (count == 65)
			SteamUserStats()->IndicateAchievementProgress("NEW_ACHIEVEMENT_1_11", 65, 130);
		if (savefile->Emblems[0] == 0xFF && savefile->Emblems[1] == 0xFF && savefile->Emblems[2] == 0xFF && savefile->Emblems[3] == 0xFF)
			SetAchievement("NEW_ACHIEVEMENT_1_7"); // The Fastest & Strongest
		if (savefile->Emblems[0xC] == 0xFF && (savefile->Emblems[0xD] & 3) == 3)
			SetAchievement("NEW_ACHIEVEMENT_1_8"); // Sub Game Master
		if ((savefile->Emblems[0xD] & 0x7C) == 0x7C)
			SetAchievement("NEW_ACHIEVEMENT_1_9"); // Chao's Best Friend
		if ((savefile->Emblems[0xE] & 0xC0) == 0xC0 && savefile->Emblems[0xF] == 0xFF && (savefile->Emblems[0x10] & 3) == 3)
			SetAchievement("NEW_ACHIEVEMENT_1_10"); // The Adventurer
	}
}

DataPointer(SaveFileData, SaveFile, 0x3B2B3A8);
void SetMetalEmblemCollected(SaveFileData *savefile, int character, signed int level, int mission)
{
	SaveFile.MetalEmblems |= 1 << (mission + 3 * (level - 1));
	if (SaveFile.MetalEmblems == 0x3FFFFFFF)
		SetAchievement("NEW_ACHIEVEMENT_1_13"); // Metal Sonic Master
}

void CheckMissions()
{
	WriteSaveFile();
	uint8_t *flags = (uint8_t *)0x3B2B368;
	for (size_t i = 0; i < 60; i++)
		if ((flags[i] & MissionFlags_Complete) == 0)
			return;
	SetAchievement("NEW_ACHIEVEMENT_1_14"); // Mission All Accomplished
}

extern "C"
{
	__declspec(dllexport) void OnExit()
	{
		SteamAPI_Shutdown();
		delete callbackobj;
	}

	__declspec(dllexport) void OnFrame()
	{
		if (SteamUserStats() && !callbackreceived)
			SteamAPI_RunCallbacks();
	}

	__declspec(dllexport) void Init(const char *path, const HelperFunctions &helperFunctions)
	{
		if (!SteamAPI_Init())
		{
			PrintDebug("SteamAchievements: Steam API failed to load.\n");
			PrintDebug("SteamAchievements: Please start Steam and restart Sonic Adventure DX to enable achievements.\n");
			return;
		}
		callbackobj = new CallbackStuff();
		if (!SteamUserStats()->RequestCurrentStats())
		{
			PrintDebug("SteamAchievements: Stats request failed.\n");
			OnExit();
			return;
		}
		WriteJump(UnlockCharacterAdventure, UnlockCharacterAdventure_r);
		WriteJump((void*)0x4130E0, sub_4130E0_r);
		WriteJump(SetEmblemCollected, SetEmblemCollected_r);
		WriteJump((void*)0x4B466C, SetMetalEmblemCollected);
		WriteCall((void*)0x59202A, CheckMissions);
	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}