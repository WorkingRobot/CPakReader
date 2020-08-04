#pragma once

#include "../CStream.h"
#include "../Objects/ECurveTableMode.h"
#include "UObject.h"

#include <unordered_map>

struct UCurveTable {
    UObject Object;
    ECurveTableMode CurveTableMode;
    std::unordered_multimap<FName, UObject> RowMap;

    friend CStream& operator>>(CStream& InputStream, UCurveTable& CurveTable);
};