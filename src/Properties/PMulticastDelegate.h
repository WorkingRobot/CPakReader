#pragma once

#include "../Exports/UObject.h"

struct PMulticastDelegate {

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PMulticastDelegate Create(CStream& InputStream, FPropertyTag& Tag) {
		// Let me know if you find a package that has a PMulticastDelegate
		LOG_ERR("Parsing of PMulticastDelegate isn't supported yet");
		return {};
	}
};
