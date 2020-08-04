#pragma once

#include "../CStream.h"

// Optional extra fields for texture platform data required by some platforms.
// Data in this struct is only serialized if the struct's value is non-default.
struct FOptTexturePlatformData {
    // Arbitrary extra data that the runtime may need.
    uint32_t ExtData;
    // Number of mips making up the mip tail, which must always be resident
    uint32_t NumMipsInTail;

    friend CStream& operator>>(CStream& InputStream, FOptTexturePlatformData& OptTexturePlatformData) {
        InputStream >> OptTexturePlatformData.ExtData;
        InputStream >> OptTexturePlatformData.NumMipsInTail;

        return InputStream;
    }
};