#include "patternscan.h"
#include "pch.h"

//Split combo pattern into mask/pattern
void Pattern::Parse(char* combo, char* pattern, char* mask)
{
	unsigned int patternLen = (strlen(combo) + 1) / 3;

	for (unsigned int i = 0; i < strlen(combo); i++)
	{
		if (combo[i] == ' ')
		{
			continue;
		}

		else if (combo[i] == '?')
		{
			mask[(i + 1) / 3] = '?';
			i += 2;
		}

		else
		{
			char byte = (char)strtol(&combo[i], 0, 16);
			pattern[(i + 1) / 3] = byte;
			mask[(i + 1) / 3] = 'x';
			i += 2;
		}
	}
	pattern[patternLen] = '\0';
	mask[patternLen] = '\0';
}

//Internal Pattern Scan
char* Pattern::In::Scan(const char* pattern, const char* mask, char* begin, unsigned int size)
{
	unsigned int patternLength = strlen(pattern);

	for (unsigned int i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (unsigned int j = 0; j < patternLength; j++)
		{
			if (mask[j] != '?' && pattern[j] != *(begin + i + j))
			{
				found = false;
				break;
			}
		}
		if (found)
		{
			return (begin + i);
		}
	}
	return nullptr;
}

char* Pattern::In::AllMods(char* combopattern)
{

}

char* Pattern::In::Proc(char* combopattern)
{

}

void Patch(char* dst, char* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_READWRITE, &oldprotect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

