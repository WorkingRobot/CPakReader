#pragma once

#include "ECompressionMethod.h"
#include "FPakFile.h"
#include "Objects/FPakEntry.h"

class CPackageFile {
public:
	CPackageFile(const FPakFile& PakFile, const FPakEntry& PakEntry) :
		PakFile(PakFile),
		Offset(PakEntry.Offset),
		Size(PakEntry.Size),
		StructSize(FPakEntry::GetSerializedSize((EPakVersion)PakFile.Info.Version, PakEntry.CompressionMethodIndex, PakEntry.CompressionBlocks.size())),
		Encrypted(PakEntry.Flags& FPakEntry::Flag_Encrypted),
		CompInfo(
			PakEntry.CompressionMethodIndex ?
				std::make_unique<CompressedInfo>(
					PakEntry.UncompressedSize,
					PakEntry.CompressionBlocks,
					PakEntry.CompressionBlockSize,
					ECompressionMethodHelper::GetType(PakFile.Info.CompressionMethods[PakEntry.CompressionMethodIndex].c_str())
				) :
			nullptr
		) {}

	struct CompressedInfo {
		CompressedInfo(int64_t UncompressedSize, const std::vector<FPakCompressedBlock>& CompressionBlocks, uint32_t CompressionBlockSize, ECompressionMethod CompressionMethod) :
			UncompressedSize(UncompressedSize),
			CompressionBlocks(CompressionBlocks),
			CompressionBlockSize(CompressionBlockSize),
			CompressionMethod(CompressionMethod) {}

		const int64_t UncompressedSize;
		const std::vector<FPakCompressedBlock> CompressionBlocks;
		const uint32_t CompressionBlockSize;
		const ECompressionMethod CompressionMethod;
	};

	const FPakFile& PakFile;

	const int64_t Offset;
	const int32_t Size;
	const uint16_t StructSize;
	const bool Encrypted;
	std::unique_ptr<const CompressedInfo> CompInfo;
};