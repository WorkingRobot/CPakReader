#pragma once

#include "../Exports/UObject.h"

struct PUInt16 {
	uint16_t Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PUInt16 Create(CStream& InputStream, FPropertyTag& Tag) {
		uint16_t Val;
		InputStream >> Val;
		return { Val };
	}
};
