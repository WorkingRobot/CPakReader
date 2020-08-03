#pragma once

#include "../Exports/UObject.h"

struct PStr {
	std::string Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PStr Create(CStream& InputStream, FPropertyTag& Tag) {
		std::string Val;
		InputStream >> Val;
		return { Val };
	}
};
