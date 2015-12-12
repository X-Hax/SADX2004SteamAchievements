// SteamAchievements.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "modloader\SADXModLoader.h"
#include "steam\steam_api.h"


extern "C"
{
	__declspec(dllexport) void Init(const char *path, const HelperFunctions &helperFunctions)
	{
		if (!SteamAPI_Init())
		{
			PrintDebug("SteamAchievements: Steam API failed to load.\n");
			return;
		}
		SteamUserStats()->RequestCurrentStats();
	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}