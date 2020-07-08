#pragma once

#include "CPackage.h"
#include "HStringHash.h"
#include "Objects/FPakEntry.h"

class CFileTree {
public:
#define SEARCH_VALUES(HashList, NameList, Value, ValueSize) \
decltype(NameList)::iterator ChildIter = NameList.end(); \
{ \
	HStringHash::HashType Hash = HStringHash::Hash(Value, ValueSize); \
	for (auto i = 0; i != NameList.size(); ++i) { \
		auto& HashData = HashList[i]; \
		if (HashData.Hash == Hash) { \
			if (HashData.NameSize == ValueSize) { \
				auto& NameData = NameList[i].first; \
				if (memcmp(NameData.get(), Value, ValueSize) == 0) { \
					ChildIter = NameList.begin() + i; \
					break; \
				} \
			} \
		} \
	} \
}

	void AddEntry(const char* Directory, const char* FilePath, const FPakFile& PakFile, const FPakEntry& PakEntry) {
		auto Separator = strchr(Directory, '/');
		if (Separator) {
			SEARCH_VALUES(FolderHashes, Folders, Directory, Separator - Directory);
			if (ChildIter == Folders.end()) {
				auto& ChildTree = Folders.emplace_back(std::make_pair(CreateKey(Directory, Separator - Directory), CFileTree()));
				FolderHashes.emplace_back(HashData(Directory, Separator - Directory));
				ChildTree.second.AddEntry(Separator + 1, FilePath, PakFile, PakEntry);
			}
			else {
				ChildIter->second.AddEntry(Separator + 1, FilePath, PakFile, PakEntry);
			}
		}
		else {
			auto ExtensionDot = strrchr(FilePath, '.');
			SEARCH_VALUES(FileHashes, Files, FilePath, ExtensionDot - FilePath);
			if (ChildIter == Files.end()) {
				auto& ChildPackage = Files.emplace_back(std::make_pair(CreateKey(FilePath, ExtensionDot - FilePath), CPackage()));
				FileHashes.emplace_back(HashData(FilePath, ExtensionDot - FilePath));
				ChildPackage.second.AddFile(ExtensionDot + 1, PakFile, PakEntry);
			}
			else {
				const_cast<CPackage&>(ChildIter->second).AddFile(ExtensionDot + 1, PakFile, PakEntry);
			}
		}
	}

	// PackagePath is without extension
	CPackage& GetPackageEntry(const char* PackagePath) {
		return *TryGetPackageEntry(PackagePath);
	}

	CPackageFile& GetFileEntry(const char* FilePath) {
		return *TryGetFileEntry(FilePath);
	}

	// PackagePath is without extension
	CPackage* TryGetPackageEntry(const char* PackagePath) {
		auto Separator = strchr(PackagePath, '/');
		if (Separator) {
			SEARCH_VALUES(FolderHashes, Folders, PackagePath, Separator - PackagePath);
			if (ChildIter == Folders.end()) {
				return nullptr;
			}
			else {
				auto& ChildTree = ChildIter->second;
				return ChildTree.TryGetPackageEntry(Separator + 1);
			}
		}
		else {
			auto FileLen = strlen(PackagePath);
			SEARCH_VALUES(FileHashes, Files, PackagePath, FileLen);
			if (ChildIter == Files.end()) {
				return nullptr;
			}
			else {
				return &ChildIter->second;
			}
		}
	}

	CPackageFile* TryGetFileEntry(const char* FilePath) {
		auto Separator = strchr(FilePath, '/');
		if (Separator) {
			SEARCH_VALUES(FolderHashes, Folders, FilePath, Separator - FilePath);
			if (ChildIter == Folders.end()) {
				return nullptr;
			}
			else {
				auto& ChildTree = ChildIter->second;
				return ChildTree.TryGetFileEntry(Separator + 1);
			}
		}
		else {
			auto ExtensionDot = strrchr(FilePath, '.');
			SEARCH_VALUES(FileHashes, Files, FilePath, ExtensionDot - FilePath);
			if (ChildIter == Files.end()) {
				return nullptr;
			}
			else {
				auto& ChildPackage = ChildIter->second;
				return ChildPackage.TryGetFile(ExtensionDot + 1);
			}
		}
	}

private:
	typedef std::unique_ptr<char[]> MapKey;

	MapKey CreateKey(const char* Name, uint32_t NameSize) {
		MapKey Key = std::make_unique<char[]>(NameSize);
		memcpy(Key.get(), Name, NameSize);
		return Key;
	}

	// The reason the name is all split up is for performance, sorry for the code readability
	struct HashData {
		HStringHash::HashType Hash;
		uint32_t NameSize;

		HashData(const char* Name, uint32_t NameSize) : NameSize(NameSize), Hash(HStringHash::Hash(Name, NameSize)) {}
	};

	std::vector<HashData> FolderHashes;
	std::vector<HashData> FileHashes;
	std::vector<std::pair<MapKey, CFileTree>> Folders;
	std::vector<std::pair<MapKey, CPackage>> Files;
};