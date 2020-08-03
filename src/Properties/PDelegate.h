#pragma once

#include "../Exports/UObject.h"

struct PDelegate {

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PDelegate Create(CStream& InputStream, FPropertyTag& Tag) {
		// Let me know if you find a package that has a PDelegate
		LOG_ERR("Parsing of PDelegate isn't supported yet");
		return {};
	}
};
