#include "UDataTable.h"

CStream& operator>>(CStream& InputStream, UDataTable& DataTable) {
	InputStream >> DataTable.Object;

	int NumRows;
	InputStream >> NumRows;
	DataTable.RowMap.reserve(NumRows);

	for (int i = 0; i < NumRows; ++i) {
		FName Key;
		UObject Value;
		InputStream >> Key;
		InputStream.PushProperty(CStream::PropStructFallback, true);
		InputStream >> Value;
		InputStream.PopProperty(CStream::PropStructFallback);
		DataTable.RowMap.emplace(Key, Value);
	}

	return InputStream;
}
