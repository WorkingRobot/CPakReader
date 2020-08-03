#pragma once

#include "../Exports/UObject.h"

struct PName {
	FName Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PName Create(CStream& InputStream, FPropertyTag& Tag) {
		FName Val;
		InputStream >> Val;
		return { Val };
	}
};
