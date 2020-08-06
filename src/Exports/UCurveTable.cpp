#include "UCurveTable.h"

CStream& operator>>(CStream& InputStream, UCurveTable& CurveTable) {
	InputStream >> CurveTable.Object;

	int NumRows;
	InputStream >> NumRows;

	uint8_t CurveModeByte;
	InputStream >> CurveModeByte;
	CurveTable.CurveTableMode = (ECurveTableMode)CurveModeByte;

	CurveTable.RowMap.reserve(NumRows);

	for (int i = 0; i < NumRows; ++i) {
		FName Key;
		UObject Value;
		InputStream >> Key;
		InputStream.PushProperty(CStream::PropStructFallback, true);
		InputStream >> Value;
		InputStream.PopProperty(CStream::PropStructFallback);
		CurveTable.RowMap.emplace(Key, Value);
	}

	return InputStream;
}
