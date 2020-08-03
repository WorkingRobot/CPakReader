#pragma once

#include "../Exports/UObject.h"

struct PLazyObject {

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PLazyObject Create(CStream& InputStream, FPropertyTag& Tag) {
		// Let me know if you find a package that has a PLazyObject
		LOG_ERR("Parsing of PLazyObject isn't supported yet");
		return {};
	}
};
