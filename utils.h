#pragma once

bool FileExists(const std::string& path);

bool GetFunctionByOrdinal(HMODULE module, WORD ordinal, std::string* name, PROC* address);
