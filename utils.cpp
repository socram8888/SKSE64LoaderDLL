
#include "pch.h"
#include "utils.h"

using std::string;
using std::stringstream;
using std::getline;

bool FileExists(const string& path) {
	DWORD attribs = GetFileAttributesA(path.c_str());
	return attribs != INVALID_FILE_ATTRIBUTES && !(attribs & FILE_ATTRIBUTE_DIRECTORY);
}

bool GetFunctionByOrdinal(HMODULE module, WORD ordinal, string* name, PROC* address) {
	ULONG size;
	PIMAGE_EXPORT_DIRECTORY exp = (PIMAGE_EXPORT_DIRECTORY)ImageDirectoryEntryToData(module, TRUE, IMAGE_DIRECTORY_ENTRY_EXPORT, &size);
	if (!exp || ordinal < exp->Base || ordinal >= exp->Base + exp->NumberOfFunctions) {
		return false;
	}

	// https://ferreirasc.github.io/PE-Export-Address-Table/ was really helpful
	WORD* ordinals = (WORD*)((BYTE*)module + exp->AddressOfNameOrdinals);
	DWORD* names = (DWORD*)((BYTE*)module + exp->AddressOfNames);
	DWORD* funcs = (DWORD*)((BYTE*)module + exp->AddressOfFunctions);

	WORD zeroBased = ordinal - (WORD)exp->Base;
	*address = (PROC)((BYTE*)module + funcs[zeroBased]);

	for (DWORD i = 0; i < exp->NumberOfNames; ++i) {
		if (ordinals[i] == zeroBased) {
			*name = (const char*)((BYTE*)module + names[i]);
			return true;
		}
	}

	*name = "<unknown>";
	return true;
}
