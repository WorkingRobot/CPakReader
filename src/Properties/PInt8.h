#pragma once

#include "../Exports/UObject.h"

struct PInt8 {
	int8_t Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PInt8 Create(CStream& InputStream, FPropertyTag& Tag) {
		int8_t Val;
		InputStream >> Val;
		return { Val };
	}
};
