#pragma once

#include "HStringHash.h"

#include <string.h>

enum class ECompressionMethod : uint8_t {
	NONE,
	ZLIB,
	GZIP,
	OODLE,

	UNKNOWN = 0xFF
};

class ECompressionMethodHelper {
public:
	static ECompressionMethod GetType(const char* extension) {
		switch (HStringHash::Crc32(extension, strlen(extension), true))
		{
#define CASE(t) case HStringHash::Crc32(#t): return ECompressionMethod::##t

			CASE(NONE);
			CASE(ZLIB);
			CASE(GZIP);
			CASE(OODLE);

#undef CASE

		default:
			return ECompressionMethod::UNKNOWN;
		}
	}
};