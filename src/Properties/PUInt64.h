#pragma once

#include "../Exports/UObject.h"

struct PUInt64 {
	uint64_t Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PUInt64 Create(CStream& InputStream, FPropertyTag& Tag) {
		uint64_t Val;
		InputStream >> Val;
		return { Val };
	}
};
