#pragma once

#include "../CStream.h"
#include "UObject.h"

#include <unordered_map>

struct UDataTable {
    UObject Object;
    std::unordered_multimap<FName, UObject> RowMap;

    friend CStream& operator>>(CStream& InputStream, UDataTable& DataTable);
};