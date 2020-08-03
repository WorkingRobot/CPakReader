#pragma once

#include "../Exports/UObject.h"

struct PInt16 {
	int16_t Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PInt16 Create(CStream& InputStream, FPropertyTag& Tag) {
		int16_t Val;
		InputStream >> Val;
		return { Val };
	}
};
