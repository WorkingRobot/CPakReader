#pragma once

#include "FPakFile.h"
#include "Objects/FPakEntry.h"

class CPackageFile {
public:
	CPackageFile(const FPakFile& PakFile, const FPakEntry& PakEntry) :
		PakFile(PakFile),
		Offset(PakEntry.Offset),
		Size(PakEntry.Size),
		StructSize(FPakEntry::GetSerializedSize((EPakVersion)PakFile.Info.Version, PakEntry.CompressionMethodIndex, PakEntry.CompressionBlocks.size())),
		Encrypted(PakEntry.Flags & FPakEntry::Flag_Encrypted),
		CompInfo(PakEntry.CompressionMethodIndex ? std::make_unique<CompressedInfo>(PakEntry.UncompressedSize, PakEntry.CompressionBlocks, PakEntry.CompressionBlockSize, PakEntry.CompressionMethodIndex) : nullptr) {}

	struct CompressedInfo {
		CompressedInfo(int64_t UncompressedSize, const std::vector<FPakCompressedBlock>& CompressionBlocks, uint32_t CompressionBlockSize, uint32_t CompressionMethodIndex) :
			UncompressedSize(UncompressedSize),
			CompressionBlocks(CompressionBlocks),
			CompressionBlockSize(CompressionBlockSize),
			CompressionMethodIndex(CompressionMethodIndex) {}

		const int64_t UncompressedSize;
		const std::vector<FPakCompressedBlock> CompressionBlocks;
		const uint32_t CompressionBlockSize;
		const uint32_t CompressionMethodIndex;
	};

	const FPakFile& PakFile;

	const int64_t Offset;
	const int32_t Size;
	const uint16_t StructSize;
	const bool Encrypted;
	std::unique_ptr<const CompressedInfo> CompInfo;
};