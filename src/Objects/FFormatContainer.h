#pragma once

#include "../CStream.h"\
#include "FByteBulkData.h"
#include "FName.h"

#include <unordered_map>

struct FFormatContainer {
    std::unordered_map<FName, FByteBulkData> Formats;
    uint32_t Alignment;

    friend CStream& operator>>(CStream& InputStream, FFormatContainer& FormatContainer) {
        // https://github.com/EpicGames/UnrealEngine/blob/master/Engine/Source/Runtime/CoreUObject/Private/Serialization/BulkData.cpp#L2121
        int NumFormats;
        InputStream >> NumFormats;
        for (int i = 0; i < NumFormats; ++i) {
            FName Name;
            InputStream >> Name;
            FByteBulkData Bulk;
            InputStream >> Bulk;
            FormatContainer.Formats.emplace(Name, Bulk);
        }

        return InputStream;
    }
};