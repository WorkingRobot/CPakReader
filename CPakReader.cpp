#include "src/CIndex.h"
#include "src/HAES.h"

int main()
{
	CIndex index;
	EErrorCode code;
	index.AddPak("J:\\Programs\\FN\\cache\\fn\\FortniteGame\\Content\\Paks\\pakChunkEarly-WindowsClient.pak", code);
	index.UseKey(FAESKey::FromHex("7f851d7e72c27cb0bfd76878e249801373dec3c40707af9a0c93fd7fc5153dbf"), FGuid::Zero(), code);
	return 0;
}
