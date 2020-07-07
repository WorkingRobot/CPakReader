#include "FPackageIndex.h"

#include "../Readers/RAssetReader.h"

// The main reason this exists is to not have cyclic headers
FObjectResource* FPackageIndex::GetResource()
{
	return ((RAssetReader*)Stream)->GetResource(*this);
}
