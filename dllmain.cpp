
#include "pch.h"
#include "utils.h"
#include "version.h"
#include "build_info.h"

using std::string;
using std::ofstream;
using std::endl;
using std::ostringstream;

static HMODULE hSkseDLL = nullptr;

bool GetSkyrimVersion(Version& parsedVersion) {
	char exePath[MAX_PATH];
	GetModuleFileNameA(NULL, exePath, sizeof(exePath));

	DWORD handle = 0;
	DWORD size = GetFileVersionInfoSizeA(exePath, &handle);
	if (size == 0) return false;

	std::unique_ptr<BYTE[]> buffer(new BYTE[size]);
	if (!GetFileVersionInfoA(exePath, 0, size, buffer.get())) {
		return false;
	}

	char* versionPos = nullptr;
	UINT len = 0;

	if (!VerQueryValueA(buffer.get(), "\\StringFileInfo\\040904B0\\ProductVersion", (LPVOID*)&versionPos, &len)) {
		return false;
	}

	try {
		parsedVersion = Version(string(versionPos));
	}
	catch (std::exception&) {
		return false;
	}

	return true;
}

string BuildSKSEDLLName(const Version& version) {
	if (version < Version(1, 6, 640)) {
		if (FileExists("skse64_steam_loader.dll")) {
			return "skse64_steam_loader.dll";
		}
		if (FileExists("sksevr_steam_loader.dll")) {
			return "sksevr_steam_loader.dll";
		}
	}

	ostringstream dllNameBuilder;
	dllNameBuilder << "skse64_" << version.major << '_' << version.minor << '_' << version.patch << ".dll";
	return dllNameBuilder.str();
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
	log << "SKSE64 Loader DLL. Git commit: " << GIT_COMMIT << ", build time " << BUILD_TIME_UTC << endl;

	Version skyrimVersion;
	if (!GetSkyrimVersion(skyrimVersion)) {
		log << "Unable to determine Skyrim version" << endl;
		return false;
	}
	log << "Skyrim version " << skyrimVersion << endl;

	string dllName = BuildSKSEDLLName(skyrimVersion);
	log << "SKSE DLL: " << dllName << ". Loading... ";
	log.flush();

	LoadLibraryFail loadLibFail;
	hSkseDLL = LoadLibraryChecked(dllName.c_str(), &loadLibFail);
	if (!hSkseDLL) {
		log << "failed with "
			<< (loadLibFail.type == LoadLibraryFailType::Exception ? "exception" : "error")
			<< " 0x" << std::hex << loadLibFail.code << endl;
		return false;
	}

	log << "OK" << endl << "Looking up first function... ";
	log.flush();

	// Get by ordinal
	string startSKSEName;
	PROC startSKSE;
	if (!GetFunctionByOrdinal(hSkseDLL, 1, &startSKSEName, &startSKSE)) {
		log << "not found?" << endl;
		FreeLibrary(hSkseDLL);
		hSkseDLL = nullptr;
		return false;
	}
	log << "OK" << endl << "Calling " << startSKSEName << "... ";
	log.flush();

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
