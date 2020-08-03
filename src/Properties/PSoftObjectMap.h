#pragma once

#include "../Exports/UObject.h"

struct PSoftObjectMap {
	FGuid Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PSoftObjectMap Create(CStream& InputStream, FPropertyTag& Tag) {
		FGuid Val;
		InputStream >> Val;
		return { Val };
	}
};
