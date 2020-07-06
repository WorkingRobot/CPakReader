#pragma once

#include "FPakFile.h"
#include "Objects/FPakEntry.h"

class CPackageFile {
public:
	CPackageFile(const FPakFile& PakFile, const FPakEntry& PakEntry) : PakFile(PakFile) {
		Offset = PakEntry.Offset;
		Size = PakEntry.Size;
		Encrypted = PakEntry.Flags & FPakEntry::Flag_Encrypted;
		if (PakEntry.CompressionMethodIndex) {
			CompInfo = std::make_unique<CompressedInfo>();
			CompInfo->UncompressedSize = PakEntry.UncompressedSize;
			CompInfo->CompressionBlocks = PakEntry.CompressionBlocks;
			CompInfo->CompressionBlockSize = PakEntry.CompressionBlockSize;
			CompInfo->CompressionMethodIndex = PakEntry.CompressionMethodIndex;
		}
	}

private:
	struct CompressedInfo {
		int64_t UncompressedSize;
		std::vector<FPakCompressedBlock> CompressionBlocks;
		uint32_t CompressionBlockSize;
		uint32_t CompressionMethodIndex;
	};

	const FPakFile& PakFile;
	
	int64_t Offset;
	int32_t Size;
	bool Encrypted;
	std::unique_ptr<CompressedInfo> CompInfo;
};