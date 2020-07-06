#pragma once

#include "CStream.h"
#include "Objects/FPakInfo.h"

struct FPakFile {
	CStream* Stream;
	FPakInfo Info;
	bool Initialized;
};