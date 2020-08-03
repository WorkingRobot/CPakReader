#pragma once

#include <cstdint>

enum ETextGender : uint8_t
{
	Masculine,
	Feminine,
	Neuter,
	// Add new enum types at the end only! They are serialized by index.
};
