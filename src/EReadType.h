#pragma once

#include <cstdint>

// Not a real enum, used for parsing properties in a UObject
enum class EReadType : uint8_t {
	NORMAL,
	MAP,
	ARRAY,

	UNKNOWN = 0xFF
};