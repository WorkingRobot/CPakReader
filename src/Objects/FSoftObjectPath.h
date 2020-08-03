#pragma once

#include "../CStream.h"
#include "FName.h"

struct FSoftObjectPath {
	// Asset path, patch to a top level object in a package. This is /package/path.assetname
	FName AssetPathName;

	// Optional FString for subobject within an asset. This is the sub path after the :
	std::string SubPathString;

	friend CStream& operator>>(CStream& InputStream, FSoftObjectPath& SoftObjectPath) {
		InputStream >> SoftObjectPath.AssetPathName;
		InputStream >> SoftObjectPath.SubPathString;

		return InputStream;
	}
};