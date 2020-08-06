#pragma once

#include "../CStream.h"
#include "../Objects/FByteBulkData.h"
#include "../Objects/FFormatContainer.h"
#include "../Objects/FStreamedAudioPlatformData.h"
#include "UObject.h"

struct USoundWave {
    UObject Object;

    bool bCooked;
    bool bStreaming;
    FByteBulkData RawData;
    FGuid CompressedDataGuid;
    std::vector<FFormatContainer> CompressedFormatData;
    FStreamedAudioPlatformData RunningPlatformData;

    friend CStream& operator>>(CStream& InputStream, USoundWave& SoundWave);
};