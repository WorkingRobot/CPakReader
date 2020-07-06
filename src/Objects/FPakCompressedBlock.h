#pragma once

#include "../CStream.h"

#include <cstdint>

struct FPakCompressedBlock {
	uint64_t CompressedStart;

	uint64_t CompressedEnd;

    friend CStream& operator>>(CStream& InputStream, FPakCompressedBlock& PakCompressedBlock) {
		InputStream >> PakCompressedBlock.CompressedStart;
		InputStream >> PakCompressedBlock.CompressedEnd;

        return InputStream;
    }
};