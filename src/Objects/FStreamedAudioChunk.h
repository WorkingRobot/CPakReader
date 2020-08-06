#pragma once

#include "../CStream.h"
#include "FByteBulkData.h"

struct FStreamedAudioChunk {
    // Size of the chunk of data in bytes including zero padding
    int DataSize;
    // Size of the audio data.
    int AudioDataSize;
    // Bulk data if stored in the package.
    FByteBulkData BulkData;

    friend CStream& operator>>(CStream& InputStream, FStreamedAudioChunk& StreamedAudioChunk) {
        // https://github.com/EpicGames/UnrealEngine/blob/28155fb6832f3294ddfcf6b5b11dc7785dcace43/Engine/Source/Runtime/Engine/Private/SoundWave.cpp#L126

        bool bCooked;
        InputStream >> bCooked;

        InputStream >> StreamedAudioChunk.BulkData;
        InputStream >> StreamedAudioChunk.DataSize;
        InputStream >> StreamedAudioChunk.AudioDataSize;

        return InputStream;
    }
};