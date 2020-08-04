#pragma once

#include "../CStream.h"

struct FStripDataFlags {
    // Serialized engine strip flags (up to 8 flags).
    uint8_t GlobalStripFlags;
    // Serialized per-class strip flags (user defined, up to 8 flags).
    uint8_t ClassStripFlags;

    friend CStream& operator>>(CStream& InputStream, FStripDataFlags& StripDataFlags) {
        InputStream >> StripDataFlags.GlobalStripFlags;
        InputStream >> StripDataFlags.ClassStripFlags;

        return InputStream;
    }
};