#pragma once

#include <cstdint>

// Not a real enum, check https://github.com/EpicGames/UnrealEngine/blob/master/Engine/Source/Runtime/PakFile/Public/IPlatformFilePak.h#L96
enum EPakVersion : int32_t
{
	PAKVER_INITIAL = 1,
	PAKVER_NO_TIMESTAMPS = 2,
	PAKVER_COMPRESSION_ENCRYPTION = 3,         // UE4.13+
	PAKVER_INDEX_ENCRYPTION = 4,               // UE4.17+ - encrypts only pak file index data leaving file content as is
	PAKVER_RELATIVE_CHUNK_OFFSETS = 5,         // UE4.20+
	PAKVER_DELETE_RECORDS = 6,                 // UE4.21+ - this constant is not used in UE4 code
	PAKVER_ENCRYPTION_KEY_GUID = 7,            // ... allows to use multiple encryption keys over the single project
	PAKVER_FNAME_BASED_COMPRESSION_METHOD = 8, // UE4.22+ - use string instead of enum for compression method
	PAKVER_FROZEN_INDEX = 9,
	PAKVER_PATH_HASH_INDEX = 10,
	PAKVER_FNV64BUGFIX = 11,

	PAKVER_LAST,
	PAKVER_INVALID,
	PAKVER_LATEST = PAKVER_LAST - 1
};
