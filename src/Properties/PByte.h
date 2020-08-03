#pragma once

#include "../Exports/UObject.h"

struct PByte {
	uint8_t Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PByte Create(CStream& InputStream, FPropertyTag& Tag) {
		switch (ReadType)
		{
		case EReadType::NORMAL:
		{
			FName Val;
			InputStream >> Val;
			return { (uint8_t)Val.Index };
		}
		case EReadType::MAP:
		{
			uint32_t Val;
			InputStream >> Val;
			return { (uint8_t)Val };
		}
		case EReadType::ARRAY:
		{
			uint8_t Val;
			InputStream >> Val;
			return { Val };
		}
		}
	}
};
