#pragma once

#include "../CStream.h"

struct FNameEntrySerialized {
	std::string Name;

	friend CStream& operator>>(CStream& InputStream, FNameEntrySerialized& NameEntrySerialized) {
		InputStream >> NameEntrySerialized.Name;
		// skip DummyHashes (case and non-case preserving hashes)
		InputStream.seek(4, CStream::Cur);

		return InputStream;
	}
};