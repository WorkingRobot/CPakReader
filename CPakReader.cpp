#include "src/CIndex.h"
#include "src/CPackageFileReader.h"
#include "src/HAES.h"

int main(int argc, char* argv[])
{
	CIndex index;
	EErrorCode code;
	for (auto& file : fs::directory_iterator("J:\\Programs\\FN\\cache\\fn\\FortniteGame\\Content\\Paks\\")) {
		index.AddPak(file.path(), code);
	}
	auto paksLoaded = index.UseKey(FAESKey::FromHex("7f851d7e72c27cb0bfd76878e249801373dec3c40707af9a0c93fd7fc5153dbf"), FGuid::Zero(), code);
	auto project = index.GetPackageFile("FortniteGame/Plugins/FortniteGame.upluginmanifest");
	CPackageFileReader reader(*project);
	auto readBuffer = std::make_unique<char[]>(reader.size());
	reader.read(readBuffer.get(), reader.size());
	printf("%s\n", readBuffer.get());
	return 0;
}
