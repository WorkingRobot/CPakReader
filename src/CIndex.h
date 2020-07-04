#pragma once

#include "EErrorCode.h"
#include "Objects/FAESKey.h"
#include "Objects/FGuid.h"
#include "Objects/FPakInfo.h"

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

class CIndex {
public:
	~CIndex();

	bool AddPak(const fs::path& FilePath, EErrorCode& ErrorCode);
	bool AddPak(CStream* InputStream, EErrorCode& ErrorCode);

	bool UseKey(const FAESKey& Key, const FGuid& Guid, EErrorCode& ErrorCode);


private:
	struct PakFile {
		CStream* Stream;
		FPakInfo Info;
	};

	std::vector<PakFile> PakFiles;
};