#pragma once

#include "pch.h"

bool FileExists(std::string path);

bool GetFunctionByOrdinal(HMODULE module, WORD ordinal, std::string* name, PROC* address);
