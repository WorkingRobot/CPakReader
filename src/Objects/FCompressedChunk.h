#pragma once

#include "../CStream.h"

struct FCompressedChunk {
    int UncompressedOffset;
    int UncompressedSize;
    int CompressedOffset;
    int CompressedSize;

    friend CStream& operator>>(CStream& InputStream, FCompressedChunk& CompressedChunk) {
        InputStream >> CompressedChunk.UncompressedOffset;
        InputStream >> CompressedChunk.UncompressedSize;
        InputStream >> CompressedChunk.CompressedOffset;
        InputStream >> CompressedChunk.CompressedSize;

        return InputStream;
    }
};