#pragma once

#include "../CStream.h"

struct FEngineVersion {
    // FEngineVersionBase
    uint16_t Major;
    uint16_t Minor;
    uint16_t Patch;
    uint32_t Changelist;

    // FEngineVersion
    std::string Branch;

    friend CStream& operator>>(CStream& InputStream, FEngineVersion& EngineVersion) {
        InputStream >> EngineVersion.Major;
        InputStream >> EngineVersion.Minor;
        InputStream >> EngineVersion.Patch;
        InputStream >> EngineVersion.Changelist;
        InputStream >> EngineVersion.Branch;

        return InputStream;
    }
};