#pragma once

#include "../Exports/UObject.h"

struct PInterface {
	// ObjectRef
	uint32_t Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PInterface Create(CStream& InputStream, FPropertyTag& Tag) {
		uint32_t Val;
		InputStream >> Val;
		return { Val };
	}
};
