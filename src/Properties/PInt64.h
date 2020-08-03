#pragma once

#include "../Exports/UObject.h"

struct PInt64 {
	int16_t Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PInt64 Create(CStream& InputStream, FPropertyTag& Tag) {
		int64_t Val;
		InputStream >> Val;
		return { Val };
	}
};
