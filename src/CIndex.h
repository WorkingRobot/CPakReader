#pragma once

#include "CFileTree.h"
#include "EErrorCode.h"
#include "FPakFile.h"
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

	int UseKey(const FAESKey& Key, const FGuid& Guid, EErrorCode& ErrorCode);

	CPackage& GetPackage(const char* Path);
	CPackageFile& GetPackageFile(const char* Path);
	CPackage* TryGetPackage(const char* Path);
	CPackageFile* TryGetPackageFile(const char* Path);

private:
	static void GetEncodedPakEntry(const char* EncodedPtr, EPakVersion Version, FPakEntry& Entry);

	std::vector<FPakFile> PakFiles;
	CFileTree FileTree;
};