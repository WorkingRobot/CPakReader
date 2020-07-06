#pragma once

#include "../CStream.h"
#include "FPakCompressedBlock.h"
#include "FSHAHash.h"

#include <cstdint>
#include <vector>

struct FPakEntry {
	static constexpr uint8_t Flag_None = 0x00;
	static constexpr uint8_t Flag_Encrypted = 0x01;
	static constexpr uint8_t Flag_Deleted = 0x02;

    int64_t Offset;

    int64_t Size;

    int64_t UncompressedSize;

	// UE4 code marks this as a uint8[20], but this is functionally the same
	FSHAHash Hash;

    std::vector<FPakCompressedBlock> CompressionBlocks;

    uint32_t CompressionBlockSize;

    uint32_t CompressionMethodIndex;

    uint8_t Flags;

	static constexpr size_t GetSerializedSize(EPakVersion Version, uint32_t CompressionMethodIndex, uint32_t CompressionBlockCount) {
		int64_t SerializedSize = sizeof(Offset) + sizeof(Size) + sizeof(UncompressedSize) + sizeof(Hash);

		if (Version >= PAKVER_FNAME_BASED_COMPRESSION_METHOD)
		{
			SerializedSize += sizeof(CompressionMethodIndex);
		}
		else
		{
			SerializedSize += sizeof(int32_t); // Old CompressedMethod var from pre-fname based compression methods
		}

		if (Version >= PAKVER_COMPRESSION_ENCRYPTION)
		{
			SerializedSize += sizeof(Flags) + sizeof(CompressionBlockSize);
			if (CompressionMethodIndex != 0)
			{
				SerializedSize += sizeof(FPakCompressedBlock) * CompressionBlockCount + sizeof(int32_t);
			}
		}
		if (Version < PAKVER_NO_TIMESTAMPS)
		{
			// Timestamp
			SerializedSize += sizeof(int64_t);
		}
		return SerializedSize;
	}

	// I don't do any pak version checking, so I assume you're using at least 4.13+ (CompressionEncryption)
	// https://github.com/EpicGames/UnrealEngine/blob/0d7ca47c39ae27a06116bf57b413f44df15d8179/Engine/Source/Runtime/PakFile/Public/IPlatformFilePak.h#L442
    friend CStream& operator>>(CStream& InputStream, FPakEntry& PakEntry) {
		InputStream >> PakEntry.Offset;
		InputStream >> PakEntry.Size;
		InputStream >> PakEntry.UncompressedSize;
		InputStream >> PakEntry.CompressionMethodIndex;
		InputStream >> PakEntry.Hash;
		if (PakEntry.CompressionMethodIndex != 0)
		{
			InputStream >> PakEntry.CompressionBlocks;
		}
		InputStream >> PakEntry.Flags;
		InputStream >> PakEntry.CompressionBlockSize;

        return InputStream;
    }
};