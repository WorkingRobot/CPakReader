#pragma once

#include "../CStream.h"
#include "FCustomVersion.h"

struct FCustomVersionContainer {
    std::vector<FCustomVersion> Versions;

    friend CStream& operator>>(CStream& InputStream, FCustomVersionContainer& CustomVersionContainer) {
        InputStream >> CustomVersionContainer.Versions;

        return InputStream;
    }
};