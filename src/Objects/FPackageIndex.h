#pragma once

#include "../CStream.h"

struct FObjectResource;

struct FPackageIndex {
    int Index;

	FObjectResource* GetResource();

	bool IsNull() {
		return Index == 0;
	}

	bool IsImport() {
		return Index < 0;
	}

	bool IsExport() {
		return Index > 0;
	}

	int ToImport() {
		return -Index - 1;
	}

	int ToExport() {
		return Index - 1;
	}

	friend CStream& operator>>(CStream& InputStream, FPackageIndex& PackageIndex) {
		InputStream >> PackageIndex.Index;
		PackageIndex.Stream = &InputStream;

		return InputStream;
	}

private:
	CStream* Stream;
};