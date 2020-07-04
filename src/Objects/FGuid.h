#pragma once

#include "../CStream.h"

#include <string.h>

struct FGuid {
	char Guid[16];

	void Invalidate() {
		memset(Guid, 0, sizeof(Guid));
	}

	friend CStream& operator>>(CStream& InputStream, FGuid& Guid) {
		return InputStream.read(Guid.Guid, sizeof(FGuid::Guid));
	}
};