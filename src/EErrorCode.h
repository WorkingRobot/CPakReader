#pragma once

#include <cstdint>

enum class EErrorCode : uint8_t {
	SUCCESS,

	CANNOT_OPEN,
	INVALID_PAKINFO,
	INVALID_PAK_KEY
};