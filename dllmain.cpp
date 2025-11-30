
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <algorithm>
#include <fstream>
#include <iostream>

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

bool LoadSKSE() {
	if (hSkseDLL) {
		return TRUE;
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

	hSkseDLL = LoadLibraryA(dllName.c_str());
	if (!hSkseDLL) {
		log << "failed: " << dllName << " (error " << GetLastError() << ")" << endl;
		return false;
	}

	log << "OK" << endl << "Calling StartSKSE... ";
	PROC startSKSE = GetProcAddress(hSkseDLL, "StartSKSE");
	if (!startSKSE) {
		log << "method not found?" << endl;
		FreeLibrary(hSkseDLL);
		hSkseDLL = nullptr;
		return false;
	}

	startSKSE();
	log << "OK " << endl << "All done!" << endl;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		return LoadSKSE();
	case DLL_PROCESS_DETACH:
		if (hSkseDLL) {
			FreeLibrary(hSkseDLL);
			hSkseDLL = nullptr;
		}
		return TRUE;
	}
	return TRUE;
}
