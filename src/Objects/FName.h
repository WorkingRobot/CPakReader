#pragma once

#include "../CStream.h"
#include "../HStringHash.h"
#include "FNameEntrySerialized.h"

struct FName {
    FNameEntrySerialized* Name;
    int Index;
    int Number;

    bool IsNull() const noexcept {
        return Name == nullptr;
    }

    const char* c_str() const {
        return Name->Name.c_str();
    }

    bool IsNone() const {
        return HStringHash::Crc32RT(c_str()) == HStringHash::Crc32("None");
    }

    friend CStream& operator>>(CStream& InputStream, FName& Name);
};