#pragma once

#include "CPackageFile.h"
#include "EAssetType.h"
#include "FPakFile.h"
#include "Objects/FPakEntry.h"

#include <memory>

class CPackage {
public:
	void AddFile(const char* Extension, const FPakFile& PakFile, const FPakEntry& PakEntry) {
		auto AssetType = EAssetTypeHelper::GetType(Extension, strlen(Extension));

		if (AssetType != EAssetType::UNKNOWN) {
			auto File = std::find_if(Files.begin(), Files.end(), [&](std::pair<EAssetType, CPackageFile>& File) {
				return File.first == AssetType;
			});
			if (File == Files.end()) {
				Files.emplace_back(std::make_pair(AssetType, CPackageFile(PakFile, PakEntry)));
			}
			else {
				//printf("File already exists!\n");
				// This value already exists!
			}
		}
		else {
			auto File = std::find_if(OtherFiles.begin(), OtherFiles.end(), [&](std::pair<MapKey, CPackageFile>& File) {
				return File.first == Extension;
			});
			if (File == OtherFiles.end()) {
				OtherFiles.emplace_back(std::make_pair(Extension, CPackageFile(PakFile, PakEntry)));
			}
			else {
				//printf("File already exists!\n");
				// This value already exists!
			}
		}
	}

	CPackageFile& GetFile(const char* Extension) {
		return *TryGetFile(Extension);
	}

	CPackageFile* TryGetFile(const char* Extension) {
		auto AssetType = EAssetTypeHelper::GetType(Extension, strlen(Extension));

		if (AssetType != EAssetType::UNKNOWN) {
			auto File = std::find_if(Files.begin(), Files.end(), [&](std::pair<EAssetType, CPackageFile>& File) {
				return File.first == AssetType;
			});
			if (File != Files.end()) {
				return &File->second;
			}
			else {
				return nullptr;
			}
		}
		else {
			auto File = std::find_if(OtherFiles.begin(), OtherFiles.end(), [&](std::pair<MapKey, CPackageFile>& File) {
				return File.first == Extension;
			});
			if (File != OtherFiles.end()) {
				return &File->second;
			}
			else {
				return nullptr;
			}
		}
	}

	void Dump(const char* FileName);

private:
	struct MapKey {
		std::unique_ptr<char[]> Name;
		uint8_t NameSize;

		MapKey(const char* Extension) {
			NameSize = strlen(Extension);
			Name = std::make_unique<char[]>(NameSize);
			memcpy(Name.get(), Extension, NameSize);
		}

		bool operator==(const char* other) const
		{
			if (NameSize != strlen(other)) {
				return false;
			}
			return !memcmp(Name.get(), other, NameSize);
		}
	};

	std::vector<std::pair<EAssetType, CPackageFile>> Files;
	std::vector<std::pair<MapKey, CPackageFile>> OtherFiles;
};