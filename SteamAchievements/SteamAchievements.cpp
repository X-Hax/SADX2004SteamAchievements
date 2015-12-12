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
			SteamUserStats()->SetAchievement("NEW_ACHIEVEMENT_1_0");
		break;
	case Characters_Knuckles:
		*(uint8_t *)0x3B1884B = 1;
		if (callbackreceived)
			SteamUserStats()->SetAchievement("NEW_ACHIEVEMENT_1_1");
		break;
	case Characters_Amy:
		*(uint8_t *)0x3B1884C = 1;
		if (callbackreceived)
			SteamUserStats()->SetAchievement("NEW_ACHIEVEMENT_1_2");
		break;
	case Characters_Gamma:
		*(uint8_t *)0x3B1884D = 1;
		if (callbackreceived)
			SteamUserStats()->SetAchievement("NEW_ACHIEVEMENT_1_3");
		break;
	case Characters_Big:
		*(uint8_t *)0x3B1884E = 1;
		if (callbackreceived)
			SteamUserStats()->SetAchievement("NEW_ACHIEVEMENT_1_4");
		break;
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
	}

	__declspec(dllexport) void Shutdown()
	{
		SteamAPI_Shutdown();
	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}