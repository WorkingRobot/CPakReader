#pragma once

#include "../CStream.h"
#include "../Objects/FStringTable.h"
#include "UObject.h"

struct UStringTable {
    UObject Object;
    // UE4 marks this as a shared ptr, but there's no need for that
    FStringTable StringTable;

    friend CStream& operator>>(CStream& InputStream, UStringTable& StringTable);
};