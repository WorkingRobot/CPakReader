#pragma once

#include "../Exports/UObject.h"

struct PFloat {
	float Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PFloat Create(CStream& InputStream, FPropertyTag& Tag) {
		float Val;
		InputStream >> Val;
		return { Val };
	}
};
