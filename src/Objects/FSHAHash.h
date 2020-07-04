#pragma once

#include "../CStream.h"

struct FSHAHash {
	char Hash[20];

	friend CStream& operator>>(CStream& InputStream, FSHAHash& SHAHash) {
		return InputStream.read(SHAHash.Hash, sizeof(FSHAHash::Hash));
	}
};
