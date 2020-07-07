#pragma once

#include "../CStream.h"

struct FGenerationInfo {
    int ExportCount;
    int NameCount;

    friend CStream& operator>>(CStream& InputStream, FGenerationInfo& GenerationInfo) {
        InputStream >> GenerationInfo.ExportCount;
        InputStream >> GenerationInfo.NameCount;

        return InputStream;
    }
};