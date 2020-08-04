#include "UDataTable.h"

CStream& operator>>(CStream& InputStream, UDataTable& DataTable) {
	InputStream >> DataTable.Object;

	int NumRows;
	InputStream >> NumRows;
	DataTable.RowMap.reserve(NumRows);

	InputStream.SetProperty(CStream::PropStructFallback, true);
	for (int i = 0; i < NumRows; ++i) {
		FName Key;
		UObject Value;
		InputStream >> Key;
		InputStream >> Value;
		DataTable.RowMap.emplace(Key, Value);
	}

	return InputStream;
}
