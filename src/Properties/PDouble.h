#pragma once

#include "../Exports/UObject.h"

struct PDouble {
	double Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PDouble Create(CStream& InputStream, FPropertyTag& Tag) {
		double Val;
		InputStream >> Val;
		return { Val };
	}
};
