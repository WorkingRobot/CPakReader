#pragma once

#include "EErrorCode.h"
#include "Objects/FAESKey.h"
#include "Objects/FGuid.h"

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class CIndex {
public:
	void AddPak(const fs::path& FilePath, EErrorCode& ErrorCode);

	void UseKey(const FAESKey& Key, const FGuid& Guid, EErrorCode& ErrorCode);


private:

};