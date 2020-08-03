#pragma once

#include "../Exports/UObject.h"

struct PEnum {
	FName Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PEnum Create(CStream& InputStream, FPropertyTag& Tag) {
		FName Val;
		InputStream >> Val;
		return { Val };
	}
};
