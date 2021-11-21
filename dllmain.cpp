#include <Windows.h>
#include <iostream>
#include "Offsets.h"

DWORD WINAPI MainThread(HMODULE hModule)
{
	DWORD moduleBase = (DWORD)GetModuleHandle("client.dll");

	while (!GetAsyncKeyState(VK_F1))
	{
		DWORD LocalPlayer = *(DWORD*)(moduleBase + dwLocalPlayer);
		DWORD GlowObjectManager = *(DWORD*)(moduleBase + dwGlowObjectManager);
		DWORD EntityList = *(DWORD*)(moduleBase + dwEntityList);

		int localTeam = *(int*)(LocalPlayer + m_iTeamNum);

		for (int i = 1; i < 32; i++)
		{
			DWORD entity = *(DWORD*)((moduleBase + dwEntityList) + 1 * 0x10);

			if (entity == NULL) continue;

			int glowIndex = *(int*)(entity + m_iGlowIndex);
			int entityTeam = *(int*)(entity + m_iTeamNum);

			if (entityTeam == localTeam)
			{
				//Local Glow
				*(float*)((GlowObjectManager + glowIndex * 0x38 + 0x8)) = 0.f; // Red
				*(float*)((GlowObjectManager + glowIndex * 0x38 + 0xC)) = 1.f; // Green
				*(float*)((GlowObjectManager + glowIndex * 0x38 + 0x10)) = 0.f; // Blue
				*(float*)((GlowObjectManager + glowIndex * 0x38 + 0x14)) = 1.7f; // Alpha
			}
			else
			{
				//Enemy Glow
				*(float*)((GlowObjectManager + glowIndex * 0x38 + 0x8)) = 1.f; // Red
				*(float*)((GlowObjectManager + glowIndex * 0x38 + 0xC)) = 0.f; // Green
				*(float*)((GlowObjectManager + glowIndex * 0x38 + 0x10)) = 0.f; // Blue
				*(float*)((GlowObjectManager + glowIndex * 0x38 + 0x14)) = 1.7f; // Alpha
			}
			*(bool*)((GlowObjectManager + glowIndex * 0x38 + 0x28)) = true;
			*(bool*)((GlowObjectManager + glowIndex * 0x38 + 0x29)) = false;

		}

	}

	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_call, LPVOID lpReserved)
{
	switch (ul_call)
	{
	case DLL_PROCESS_ATTACH:
			CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));
	case DLl_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}