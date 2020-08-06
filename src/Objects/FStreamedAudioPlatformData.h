#pragma once

#include "../CStream.h"
#include "FName.h"
#include "FStreamedAudioChunk.h"

struct FStreamedAudioPlatformData {
    int NumChunks;
    FName AudioFormat;
    std::vector<FStreamedAudioChunk> Chunks;

    friend CStream& operator>>(CStream& InputStream, FStreamedAudioPlatformData& StreamedAudioPlatformData) {
        InputStream >> StreamedAudioPlatformData.NumChunks;
        InputStream >> StreamedAudioPlatformData.AudioFormat;

        StreamedAudioPlatformData.Chunks.reserve(StreamedAudioPlatformData.NumChunks);
        for (int i = 0; i < StreamedAudioPlatformData.NumChunks; ++i) {
            auto& Chunk = StreamedAudioPlatformData.Chunks.emplace_back();
            InputStream >> Chunk;
        }

        return InputStream;
    }
};