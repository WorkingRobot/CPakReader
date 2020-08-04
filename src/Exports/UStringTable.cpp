#include "UStringTable.h"

CStream& operator>>(CStream& InputStream, UStringTable& StringTable) {
	InputStream >> StringTable.Object;

	InputStream >> StringTable.StringTable;

	return InputStream;
}
