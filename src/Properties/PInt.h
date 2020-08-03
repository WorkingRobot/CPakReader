#pragma once

#include "../Exports/UObject.h"

struct PInt {
	int Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PInt Create(CStream& InputStream, FPropertyTag& Tag) {
		int Val;
		InputStream >> Val;
		return { Val };
	}
};
