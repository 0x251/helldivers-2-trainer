#include "pch.h"
#include "windows.h"
#include "iostream"
#include "string"
#include <Psapi.h>

namespace Memory
{
	bool CompareByteArray(uintptr_t dest, std::string pattern)
	{
		for (DWORD i = 0; i < pattern.length(); i++)
		{
			if (pattern[i] == '?')
			{
				continue;
			}

			if (((char*)dest)[i] != pattern[i])
			{
				return false;
			}
		}

		return true;
	}

	uintptr_t FindPattern(std::string moduleName, std::string pattern)
	{

		if (HMODULE hModule = GetModuleHandleA(moduleName.c_str()))
		{

			MODULEINFO moduleInfo = {};
			if (GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(moduleInfo)))
			{

				for (uintptr_t i = 0; i < moduleInfo.SizeOfImage - pattern.length(); i++)
				{
					if (CompareByteArray((uintptr_t)moduleInfo.lpBaseOfDll + i, pattern))
					{

						return (uintptr_t)moduleInfo.lpBaseOfDll + i;
					}
				}
			}
		}


		return 0;
	}

	uintptr_t RelativeRIP(uintptr_t address, int instruction)
	{
		return address + instruction + *(int*)(address + instruction - 4);
	}

}