#pragma once

#include <cstdint>

enum EFormatArgumentType : int8_t
{
	Int,
	UInt,
	Float,
	Double,
	Text,
	Gender,
	// Add new enum types at the end only! They are serialized by index.
};
