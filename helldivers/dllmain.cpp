
#include "pch.h"
#include <Windows.h>
#include <cstdio>
#include "Memory.hpp"
#include <iostream>




void CreateConsole() {
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	SetConsoleTitle("HellDivers2 Internal");
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
}

void PatchAddress(uintptr_t address, SIZE_T size, BYTE* patch)
{
	DWORD oldProtect;
	VirtualProtect(reinterpret_cast<LPVOID>(address), size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(reinterpret_cast<LPVOID>(address), patch, size);
	VirtualProtect(reinterpret_cast<LPVOID>(address), size, oldProtect, &oldProtect);
}


void AllocateMem(uintptr_t address, SIZE_T size) {
	LPVOID memory = VirtualAlloc(reinterpret_cast<LPVOID>(address), size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
}

void Godmode(uintptr_t healthAddress)
{
	// Broken

	BYTE patch[] = {
		0xC7, 0x07, 0x64, 0x00, 0x00, 0x00,  // mov dword ptr [rdi], 100
		0x48, 0x8B, 0x84, 0x9E, 0x28, 0x04, 0x00,  // mov rax, [rsi+rbx*8+428]
		0x8B, 0x88, 0x10, 0x00, 0x00, 0x00,        // mov ecx, [rax+10]
		0xE9, 0x0C, 0x00, 0x00, 0x00               // jmp return
	};
	DWORD oldProtect;
	VirtualProtect(reinterpret_cast<LPVOID>(healthAddress), sizeof(patch), PAGE_READWRITE, &oldProtect);
	memcpy(reinterpret_cast<void*>(healthAddress), patch, sizeof(patch));
	VirtualProtect(reinterpret_cast<LPVOID>(healthAddress), sizeof(patch), oldProtect, &oldProtect);
}

DWORD WINAPI MainThread(LPVOID lpParam) {
    CreateConsole();
    printf("[DEBUG] - Waiting for helldivers 2 to open...\n");
	HMODULE hDXGIDLL = 0;
	do
	{
		hDXGIDLL = GetModuleHandle("d3d11.dll"); // check if game window is actually running
		Sleep(4000);
	} while (!hDXGIDLL);
	Sleep(100);

    printf("[DEBUG] - Detected helldivers \n");

	HMODULE hModule = GetModuleHandle("game.dll");
	if (hModule != NULL) {
		printf("[DEBUG] - game.dll Address: %p\n", hModule);
	}
	else {
		printf("[DEBUG] - Failed to get the address of game.dll\n");
	}

	printf("[DEBUG] - d3d11.dll Address: %p\n", hDXGIDLL); 

	uintptr_t AmmoAddress = Memory::FindPattern("game.dll", "\x41\x83\x2C\xC2\x01\x4D\x8D\x04\xC2\x49\x8B\x84\xCA");

	printf("[DEBUG] Ammo address - %p\n", AmmoAddress);

	BYTE AmmoPatch[] = { 0xDB, 0x00 };
	
	PatchAddress(AmmoAddress, 2, AmmoPatch); // This patch will only work on the first gun, so if you change guns game = crash kinda sus

	printf("[Enabled] Inf Ammo\n");

	//uintptr_t Health = Memory::FindPattern("game.dll", "\x45\x89\x38\x49\x8B\x84\xDE\x28\x04??\x8B\x48\x10");
	//printf("[DEBUG] HP Address - %p\n", Health);

	//Godmode(Health) Broken and TO LAZY to figure it out probs have to alloc make a new jump but yea 

	//uintptr_t Syringes = Memory::FindPattern("game.dll", "\x41\xFF\xCF\x3B\xC2\x74\x61"); 

	//BYTE SyringesPatch[] = {
	//	0x90, 0x90, 0x90 
	//};

	//printf("[DEBUG] Syringes Address: - %p\n", Syringes);

	//PatchAddress(Syringes, 3, SyringesPatch);

    return 0;

}



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
		DisableThreadLibraryCalls(hModule);
		CreateThread(nullptr, 0, MainThread, NULL, 0, nullptr);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

