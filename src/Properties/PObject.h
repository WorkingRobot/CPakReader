#pragma once

#include "../Exports/UObject.h"
#include "../Objects/FPackageIndex.h"

struct PObject {
	FPackageIndex Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PObject Create(CStream& InputStream, FPropertyTag& Tag) {
		FPackageIndex Val;
		InputStream >> Val;
		return { Val };
	}
};
