#pragma once

#include "../CStream.h"

#include <compare>
#include <string.h>

struct FGuid {
	static constexpr FGuid Zero() {
		return FGuid(0, 0, 0, 0);
	}

	uint32_t A;
	uint32_t B;
	uint32_t C;
	uint32_t D;

	constexpr FGuid() : FGuid(0, 0, 0, 0) {}
	constexpr FGuid(uint32_t A, uint32_t B, uint32_t C, uint32_t D) : A(A), B(B), C(C), D(D) {}

	void Invalidate() {
		A = 0;
		B = 0;
		C = 0;
		D = 0;
	}

	auto operator<=>(const FGuid&) const = default;

	friend CStream& operator>>(CStream& InputStream, FGuid& Guid) {
		InputStream >> Guid.A;
		InputStream >> Guid.B;
		InputStream >> Guid.C;
		InputStream >> Guid.D;
		return InputStream;
	}
};