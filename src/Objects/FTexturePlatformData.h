#pragma once

#include "../CStream.h"
#include "EPixelFormat.h"
#include "FOptTexturePlatformData.h"
#include "FTexture2DMipMap.h"
#include "FVirtualTextureBuiltData.h"

struct FTexturePlatformData {
    int64_t DataOffset;

    int SizeX;
    int SizeY;
    // 31: CubeMap, 30: HasOptData, 29-0: NumSlices
    uint32_t PackedData;
    EPixelFormat PixelFormat;
    FOptTexturePlatformData OptData;
    std::vector<FTexture2DMipMap> Mips;
    FVirtualTextureBuiltData VTData;

    bool HasCubeMap() const {
        return PackedData & (1u << 31u);
    }

    bool HasOptData() const {
        return PackedData & (1u << 30u);
    }

    int GetNumSlices() const {
        return PackedData & ((1u << 30u) - 1u);
    }

    friend CStream& operator>>(CStream& InputStream, FTexturePlatformData& TexturePlatformData) {
        // https://github.com/EpicGames/UnrealEngine/blob/283e412aa843210f2d6e9ed0236861cf749b3429/Engine/Source/Runtime/Engine/Private/TextureDerivedData.cpp#L1256

        InputStream >> TexturePlatformData.SizeX;
        InputStream >> TexturePlatformData.SizeY;
        InputStream >> TexturePlatformData.PackedData;
        std::string PixelFormatString;
        InputStream >> PixelFormatString;
        TexturePlatformData.PixelFormat = EPixelFormatHelper::GetType(PixelFormatString.c_str());

        if (TexturePlatformData.HasOptData()) {
            InputStream >> TexturePlatformData.OptData;
        }

        {
            // unused in loading
            int FirstMipToSerialize;
            InputStream >> FirstMipToSerialize;
        }

        // TODO: add some property system to CStream PLEASE
        int MipCount;
        InputStream >> MipCount;
        TexturePlatformData.Mips.reserve(MipCount);
        for (int i = 0; i < MipCount; ++i) {
            auto& Item = TexturePlatformData.Mips.emplace_back();
            Item.DataOffset = TexturePlatformData.DataOffset;
            InputStream >> Item;
        }

        bool bIsVirtual;
        InputStream >> bIsVirtual;
        if (bIsVirtual) {
            InputStream >> TexturePlatformData.VTData;
        }

        return InputStream;
    }
};
