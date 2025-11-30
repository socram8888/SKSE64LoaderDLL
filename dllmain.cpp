
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

static HMODULE hSkseDLL = nullptr;

string GetSkyrimVersion() {
	char exePath[MAX_PATH];
	GetModuleFileNameA(NULL, exePath, sizeof(exePath));

	DWORD handle = 0;
	DWORD size = GetFileVersionInfoSizeA(exePath, &handle);
	if (size == 0) return "";

	BYTE* buffer = new BYTE[size];
	if (!GetFileVersionInfoA(exePath, 0, size, buffer)) {
		delete[] buffer;
		return "";
	}

	char* versionPos = nullptr;
	UINT len = 0;

	if (!VerQueryValueA(buffer, "\\StringFileInfo\\040904B0\\ProductVersion", (LPVOID*)&versionPos, &len)) {
		delete[] buffer;
		return "";
	}

	string version(versionPos);
	delete[] buffer;
	return version;
}

string BuildSKSEDLLName(string version) {
	string dllName = "skse64_";
	int dotCount = 0;

	for (char c : version) {
		if (c == '.') {
			if (dotCount == 2) break;
			dllName += "_";
			dotCount++;
		}
		else {
			dllName += c;
		}
	}

	dllName += ".dll";
	return dllName;
}

enum class LoadLibraryFailType {
	Error,
	Exception
};

typedef struct {
	LoadLibraryFailType type;
	unsigned long code;
} LoadLibraryFail;

// Need this pure C function without any C++ shit to avoid "C2712: cannot use __try in functions that require object unwinding"
HMODULE LoadLibraryChecked(const char *dllName, LoadLibraryFail* fail) {
	__try {
		HMODULE lib = LoadLibraryA(dllName);
		if (!lib) {
			fail->type = LoadLibraryFailType::Error;
			fail->code = GetLastError();
		}
		return lib;
	}
	__except (1) {
		fail->type = LoadLibraryFailType::Exception;
		fail->code = GetExceptionCode();
		return nullptr;
	}
}

bool LoadSKSE() {
	if (hSkseDLL) {
		return true;
	}

	ofstream log("skse64_loader_dll.log");
	string version = GetSkyrimVersion();
	if (version.empty()) {
		log << "Unable to determine Skyrim version" << endl;
		return false;
	}
	log << "Skyrim version: " << version << endl;

	string dllName = BuildSKSEDLLName(version);
	log << "SKSE64 DLL: " << dllName << ". Loading... ";
	log.flush();

	LoadLibraryFail loadLibFail;
	hSkseDLL = LoadLibraryChecked(dllName.c_str(), &loadLibFail);
	if (!hSkseDLL) {
		log << "failed with " << (loadLibFail.type == LoadLibraryFailType::Exception ? "exception" : "error") << " 0x" << hex << setw(8) << loadLibFail.code << endl;
		return false;
	}

	log << "OK" << endl << "Calling StartSKSE... ";
	log.flush();

	PROC startSKSE = GetProcAddress(hSkseDLL, "StartSKSE");
	if (!startSKSE) {
		log << "method not found?" << endl;
		FreeLibrary(hSkseDLL);
		hSkseDLL = nullptr;
		return false;
	}

	startSKSE();
	log << "OK " << endl << "All done!" << endl;
	return true;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	(void)hModule;
	(void)lpReserved;

	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		return LoadSKSE();
	case DLL_PROCESS_DETACH:
		if (hSkseDLL) {
			FreeLibrary(hSkseDLL);
			hSkseDLL = nullptr;
		}
		return true;
	}
	return true;
}
