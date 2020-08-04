#pragma once

#include "../CStream.h"
#include "FByteBulkData.h"

struct FTexture2DMipMap {
	int64_t DataOffset;

	int SizeX;
	int SizeY;
	int SizeZ;
	FByteBulkData BulkData;

    friend CStream& operator>>(CStream& InputStream, FTexture2DMipMap& Texture2DMipMap) {
		// https://github.com/EpicGames/UnrealEngine/blob/283e412aa843210f2d6e9ed0236861cf749b3429/Engine/Source/Runtime/Engine/Private/Texture2D.cpp#L413

		bool bCooked;
		InputStream >> bCooked;

		Texture2DMipMap.BulkData.DataOffset = Texture2DMipMap.DataOffset;
		InputStream >> Texture2DMipMap.BulkData;
		InputStream >> Texture2DMipMap.SizeX;
		InputStream >> Texture2DMipMap.SizeY;
		InputStream >> Texture2DMipMap.SizeZ;

        return InputStream;
    }
};