#include "UTexture2D.h"

#include "../Objects/FStripDataFlags.h"

CStream& operator>>(CStream& InputStream, UTexture2D& Texture2D) {
	InputStream >> Texture2D.Object;

	FStripDataFlags Flags;
	InputStream >> Flags; // UTexture: https://github.com/EpicGames/UnrealEngine/blob/283e412aa843210f2d6e9ed0236861cf749b3429/Engine/Source/Runtime/Engine/Private/Texture.cpp#L334
	InputStream >> Flags; // UTexture2D: https://github.com/EpicGames/UnrealEngine/blob/283e412aa843210f2d6e9ed0236861cf749b3429/Engine/Source/Runtime/Engine/Private/Texture2D.cpp#L468

	bool bCooked;
	InputStream >> bCooked;

	if (bCooked) {
		// https://github.com/EpicGames/UnrealEngine/blob/283e412aa843210f2d6e9ed0236861cf749b3429/Engine/Source/Runtime/Engine/Private/TextureDerivedData.cpp#L2049

		FName PixelFormatName;
		InputStream >> PixelFormatName;
		while (!PixelFormatName.IsNone()) {
			EPixelFormat PixelFormat = EPixelFormatHelper::GetType(PixelFormatName.c_str());
			int64_t SkipOffset;
			InputStream >> SkipOffset;

			auto& TextureData = Texture2D.PlatformDatas.emplace_back();
			InputStream >> TextureData;

			InputStream >> PixelFormatName;
		}
	}

	return InputStream;
}
