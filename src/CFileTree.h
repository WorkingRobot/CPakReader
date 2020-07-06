#pragma once

#include "CPackage.h"
#include "HStringHash.h"
#include "Objects/FPakEntry.h"

class CFileTree {
public:
	void AddEntry(const char* Directory, const char* FilePath, const FPakFile& PakFile, const FPakEntry& PakEntry) {
		auto Separator = strchr(Directory, '/');
		if (Separator) {
			auto ChildIter = std::find_if(Folders.begin(), Folders.end(), [&](std::pair<MapKey, CFileTree>& Folder) {
				return Folder.first.IsEqual(Directory, Separator - Directory);
			});
			if (ChildIter == Folders.end()) {
				auto& ChildTree = Folders.emplace_back(std::make_pair(MapKey(Directory, Separator - Directory), CFileTree())).second;
				ChildTree.AddEntry(Separator + 1, FilePath, PakFile, PakEntry);
			}
			else {
				auto& ChildTree = ChildIter->second;
				ChildTree.AddEntry(Separator + 1, FilePath, PakFile, PakEntry);
			}
		}
		else {
			auto ExtensionDot = strrchr(FilePath, '.');
			auto ChildIter = std::find_if(Files.begin(), Files.end(), [&](std::pair<MapKey, CPackage>& File) {
				return File.first.IsEqual(FilePath, ExtensionDot - FilePath);
			});
			if (ChildIter == Files.end()) {
				auto& ChildPackage = Files.emplace_back(std::make_pair(MapKey(FilePath, ExtensionDot - FilePath), CPackage())).second;
				ChildPackage.AddFile(ExtensionDot + 1, PakFile, PakEntry);
			}
			else {
				auto& ChildPackage = ChildIter->second;
				ChildPackage.AddFile(ExtensionDot + 1, PakFile, PakEntry);
			}
		}
	}

private:
	struct MapKey {
		std::unique_ptr<char[]> Name;
		size_t NameSize;

		MapKey(const char* Name, size_t NameSize) : NameSize(NameSize) {
			this->Name = std::make_unique<char[]>(NameSize);
			memcpy(this->Name.get(), Name, NameSize);
		}

		bool IsEqual(const char* Directory, size_t DirectorySize) {
			if (NameSize != DirectorySize) {
				return false;
			}
			return !memcmp(Name.get(), Directory, NameSize);
		}
	};

	std::vector<std::pair<MapKey, CFileTree>> Folders;
	std::vector<std::pair<MapKey, CPackage>> Files;
};