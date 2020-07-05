#pragma once

#include "../CStream.h"

struct FAESKey {
	char Key[32];

	static FAESKey FromHex(const char HexBlob[64]) {
		FAESKey Key;
		for (int i = 0, j = 0; j < 32; i += 2, j++) {
			Key.Key[j] = (HexBlob[i] % 32 + 9) % 25 * 16 + (HexBlob[i + 1] % 32 + 9) % 25;
		}
		return Key;
	}

	friend CStream& operator>>(CStream& InputStream, FAESKey& Key) {
		return InputStream.read(Key.Key, sizeof(FAESKey::Key));
	}
};