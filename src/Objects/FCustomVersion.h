#pragma once

#include "../CStream.h"
#include "FGuid.h"

struct FCustomVersion {
    FGuid Key;
    int Version;

    friend CStream& operator>>(CStream& InputStream, FCustomVersion& CustomVersion) {
        InputStream >> CustomVersion.Key;
        InputStream >> CustomVersion.Version;

        return InputStream;
    }
};