#pragma once

#include "../CStream.h"
#include "FNameEntrySerialized.h"

struct FName {
    FNameEntrySerialized* Name;
    int Index;
    int Number;

    friend CStream& operator>>(CStream& InputStream, FName& Name);
};