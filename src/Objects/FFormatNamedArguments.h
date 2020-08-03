#pragma once

#include "../CStream.h"
#include "FFormatArgumentValue.h"

// UE4 defines this as a TMap<FString, FFormatArgumentValue>
// Maps take a lot of memory, so a vector of pairs will do
struct FFormatNamedArguments {
    std::vector<std::pair<std::string, FFormatArgumentValue>> Value;

    friend CStream& operator>>(CStream& InputStream, FFormatNamedArguments& FormatNamedArguments) {
        InputStream >> FormatNamedArguments.Value;

        return InputStream;
    }
};