#include "CIndex.h"

#include <fstream>

void CIndex::AddPak(const fs::path& FilePath, EErrorCode& ErrorCode)
{
	std::ifstream FileStream(FilePath);
	if (FileStream.bad()) {
		ErrorCode = EErrorCode::CANNOT_OPEN;
		return;
	}


}

void CIndex::UseKey(const FAESKey& Key, const FGuid& Guid, EErrorCode& ErrorCode)
{
}
