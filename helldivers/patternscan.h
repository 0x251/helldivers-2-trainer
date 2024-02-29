#pragma once
#include <Windows.h>
#include <TlHelp32.h>


namespace Pattern
{
	void Parse(char* combo, char* pattern, char* mask);

	namespace In
	{
		char* Pattern::In::Scan(const char* pattern, const char* mask, char* begin, unsigned int size);

		char* Mod(char* combopattern, HMODULE* module);

		char* AllMods(char* combopattern);

		char* Proc(char* combopattern);
	}

	void Patch(char* dst, char* src, unsigned int size); 

}