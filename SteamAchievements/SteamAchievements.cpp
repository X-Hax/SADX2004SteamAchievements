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

void UnlockCharacterAdventure_r(int character)
{
	switch (character)
	{
	case Characters_Sonic:
		*(uint8_t *)0x3B18849 = 1;
		break;
	case Characters_Tails:
		*(uint8_t *)0x3B1884A = 1;
		if (callbackreceived)
			SteamUserStats()->SetAchievement("NEW_ACHIEVEMENT_1_0"); // Miles "Tails" Prower
		break;
	case Characters_Knuckles:
		*(uint8_t *)0x3B1884B = 1;
		if (callbackreceived)
			SteamUserStats()->SetAchievement("NEW_ACHIEVEMENT_1_1"); // Knuckles the Echidna
		break;
	case Characters_Amy:
		*(uint8_t *)0x3B1884C = 1;
		if (callbackreceived)
			SteamUserStats()->SetAchievement("NEW_ACHIEVEMENT_1_2"); // Amy Rose
		break;
	case Characters_Gamma:
		*(uint8_t *)0x3B1884D = 1;
		if (callbackreceived)
			SteamUserStats()->SetAchievement("NEW_ACHIEVEMENT_1_3"); // E-102 "Gamma"
		break;
	case Characters_Big:
		*(uint8_t *)0x3B1884E = 1;
		if (callbackreceived)
			SteamUserStats()->SetAchievement("NEW_ACHIEVEMENT_1_4"); // Big the Cat
		break;
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
			SteamUserStats()->SetAchievement("NEW_ACHIEVEMENT_1_11"); // The Perfect Adventurer
			SteamUserStats()->SetAchievement("NEW_ACHIEVEMENT_1_12"); // Metal Sonic
		}
		else if (count == 65)
			SteamUserStats()->IndicateAchievementProgress("NEW_ACHIEVEMENT_1_11", 65, 130);
		if (savefile->Emblems[0] == 0xFF && savefile->Emblems[1] == 0xFF && savefile->Emblems[2] == 0xFF && savefile->Emblems[3] == 0xFF)
			SteamUserStats()->SetAchievement("NEW_ACHIEVEMENT_1_7"); // The Fastest & Strongest
		if (savefile->Emblems[0xC] == 0xFF && (savefile->Emblems[0xD] & 3) == 3)
			SteamUserStats()->SetAchievement("NEW_ACHIEVEMENT_1_8"); // Sub Game Master
		if ((savefile->Emblems[0xD] & 0x7C) == 0x7C)
			SteamUserStats()->SetAchievement("NEW_ACHIEVEMENT_1_9"); // Chao's Best Friend
		if ((savefile->Emblems[0xE] & 0xC0) == 0xC0 && savefile->Emblems[0xF] == 0xFF && (savefile->Emblems[0x10] & 3) == 3)
			SteamUserStats()->SetAchievement("NEW_ACHIEVEMENT_1_10"); // The Adventurer
	}
}

extern "C"
{
	__declspec(dllexport) void Init(const char *path, const HelperFunctions &helperFunctions)
	{
		if (!SteamAPI_Init())
		{
			PrintDebug("SteamAchievements: Steam API failed to load.\n");
			PrintDebug("SteamAchievements: Please start Steam and restart Sonic Adventure DX to enable achievements.\n");
			return;
		}
		callbackobj = new CallbackStuff();
		SteamUserStats()->RequestCurrentStats();
		WriteJump(UnlockCharacterAdventure, UnlockCharacterAdventure_r);
		WriteJump(SetEmblemCollected, SetEmblemCollected_r);
	}

	__declspec(dllexport) void OnExit()
	{
		SteamAPI_Shutdown();
	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}