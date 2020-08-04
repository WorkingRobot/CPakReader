#pragma once

#include "../CStream.h"
#include "../Objects/FTexturePlatformData.h"
#include "UObject.h"

struct UTexture2D {
    int64_t DataOffset;

    UObject Object;
    std::vector<FTexturePlatformData> PlatformDatas;

    friend CStream& operator>>(CStream& InputStream, UTexture2D& Texture2D);
};