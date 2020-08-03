#pragma once

#include "../Exports/UObject.h"

#include <any>

struct PSet {
	std::vector<std::any> Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PSet Create(CStream& InputStream, FPropertyTag& Tag) {
		int NumKeysToRemove;
		InputStream >> NumKeysToRemove;

		if (NumKeysToRemove) {
			// Never seen this before
			LOG_ERR("Parsing of PSet with NumKeysToRemove isn't supported yet");
		}

		int NumEntries;
		InputStream >> NumEntries;

		std::vector<std::any> Val;
		Val.reserve(NumEntries);
		for (int i = 0; i < NumEntries; ++i) {
			Val.emplace_back(UObject::ReadProperty<EReadType::ARRAY>(InputStream, Tag, Tag.InnerType));
		}

		return { Val };
	}
};
