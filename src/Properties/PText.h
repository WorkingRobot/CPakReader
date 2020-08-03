#pragma once

#include "../Exports/UObject.h"
#include "../Objects/FText.h"

struct PText {
	FText Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PText Create(CStream& InputStream, FPropertyTag& Tag) {
		FText Val;
		InputStream >> Val;
		return { Val };
	}
};
