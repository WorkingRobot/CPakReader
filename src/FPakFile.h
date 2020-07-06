#pragma once

#include "CStream.h"
#include "Objects/FAESKey.h"
#include "Objects/FPakInfo.h"

struct FPakFile {
	CStream* Stream;
	FPakInfo Info;
	FAESKey Key;
	bool Initialized;
};