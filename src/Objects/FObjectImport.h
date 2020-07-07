#pragma once

#include "../CStream.h"
#include "FObjectResource.h"

struct FObjectImport : public FObjectResource {
    FName ClassPackage;
    FName ClassName;

	friend CStream& operator>>(CStream& InputStream, FObjectImport& ObjectImport) {
		InputStream >> ObjectImport.ClassPackage;
		InputStream >> ObjectImport.ClassName;
		InputStream >> ObjectImport.OuterIndex;
		InputStream >> ObjectImport.ObjectName;

		return InputStream;
	}
};