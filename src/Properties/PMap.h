#pragma once

#include "../Exports/UObject.h"

#include <any>
#include <unordered_map>

struct PMap {
	std::vector<std::pair<std::any, std::any>> Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PMap Create(CStream& InputStream, FPropertyTag& Tag) {
		int NumKeysToRemove;
		InputStream >> NumKeysToRemove;

		if (NumKeysToRemove) {
			// Never seen this before
			LOG_ERR("Parsing of PMap with NumKeysToRemove isn't supported yet");
		}

		int NumEntries;
		InputStream >> NumEntries;

		std::vector<std::pair<std::any, std::any>> Val;
		Val.reserve(NumEntries);
		for (int i = 0; i < NumEntries; ++i) {
			Val.emplace_back(UObject::ReadProperty<EReadType::MAP>(InputStream, Tag, Tag.InnerType), UObject::ReadProperty<EReadType::MAP>(InputStream, Tag, Tag.ValueType));
		}

		return { Val };
	}
};
