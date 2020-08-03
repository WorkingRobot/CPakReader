#include "FName.h"

#include "../Readers/RAssetReader.h"

// The main reason this exists is to not have cyclic headers and to populate the Name pointer
CStream& operator>>(CStream& InputStream, FName& Name) {
	InputStream >> Name.Index;
	InputStream >> Name.Number;

	((RAssetReader&)InputStream).PopulateName(Name);

	return InputStream;
}