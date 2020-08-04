#pragma once

#include "../CStream.h"
#include "EPixelFormat.h"
#include "EVirtualTextureCodec.h"
#include "FByteBulkData.h"

// Serialized by FVirtualTextureBuiltData
struct FVirtualTextureDataChunk {
    FByteBulkData BulkData;
    uint32_t SizeInBytes;
    uint32_t CodecPayloadSize;
    uint16_t CodecPayloadOffset[8];
    EVirtualTextureCodec CodecType[8];
};
