#pragma once

#include "FName.h"
#include "FPackageIndex.h"

struct FObjectResource {
    FName ObjectName;
    FPackageIndex OuterIndex;
};