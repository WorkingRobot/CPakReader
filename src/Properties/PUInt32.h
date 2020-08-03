#pragma once

#include "../Exports/UObject.h"

struct PUInt32 {
	uint32_t Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PUInt32 Create(CStream& InputStream, FPropertyTag& Tag) {
		uint32_t Val;
		InputStream >> Val;
		return { Val };
	}
};
