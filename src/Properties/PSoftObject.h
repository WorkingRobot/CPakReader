#pragma once

#include "../Exports/UObject.h"
#include "../Objects/FSoftObjectPath.h"

struct PSoftObject {
	FSoftObjectPath Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PSoftObject Create(CStream& InputStream, FPropertyTag& Tag) {
		FSoftObjectPath Val;
		InputStream >> Val;

		return { Val };
	}
};
