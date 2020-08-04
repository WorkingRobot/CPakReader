#include "src/CIndex.h"
#include "src/CPackageFileStream.h"
#include "src/HAES.h"
#include "src/Readers/RAssetReader.h"

#include <chrono>
#include <optional>
#include "src/Exports/UObject.h"
#include "src/Properties/PArray.h"
#include "src/Properties/PObject.h"
#include "src/Exports/UTexture2D.h"

namespace ch = std::chrono;

int main(int argc, char* argv[])
{
	FAESKey Key = FAESKey::FromHex("642ca35c21741bb86feb40ce220693b60b04eea685285e0a2d553d4631fd2aec");
	fs::path DirPath = "J:\\Programs\\FN\\cache\\fn\\FortniteGame\\Content\\Paks\\";
	if (argc >= 3) {
		DirPath = argv[1];
		Key = FAESKey::FromHex(argv[2]);
	}

	CIndex index;
	EErrorCode code;
	for (auto& file : fs::directory_iterator(DirPath)) {
		index.AddPak(file.path(), code);
	}
	auto start = ch::steady_clock::now();
	int paksLoaded = index.UseKey(Key, FGuid::Zero(), code);
	auto end = ch::steady_clock::now();
	printf("Loaded %d paks in %.2f ms\n", paksLoaded, (end - start).count() / 1000000.f);
	auto& pkg = index.GetPackage("FortniteGame/Plugins/FortWater/Content/Textures/Swamp_Mask");//"FortniteGame/Content/Athena/Items/Cosmetics/Characters/CID_547_Athena_Commando_F_Meteorwoman");
	pkg.Dump("versefn");
	RAssetReader reader(pkg);
	for (auto& n : reader.Exports) {
		printf("%s\n", n.type().name());
		auto obj = std::any_cast<UTexture2D>(&n);
		for (auto& p : obj->Object.Properties) {
			printf(" %s: %s\n", p.first.c_str(), p.second.type().name());
			if (p.first == "ItemVariants") {
				auto arr = std::any_cast<PArray>(&p.second);
				printf("  Length: %d\n", arr->Value.size());
				for (auto& a : arr->Value) {
					printf("  %s: ", a.type().name());
					auto o = std::any_cast<PObject>(&a);
					printf("%s\n", o->Value.GetResource()->ObjectName.c_str());
				}
			}
		}
	}

	/*
	auto project = index.GetPackageFile("FortniteGame/Plugins/FortniteGame.upluginmanifest");
	CPackageFileStream reader(*project);
	auto readBuffer = std::make_unique<char[]>(reader.size());
	reader.read(readBuffer.get(), reader.size());
	printf("%s\n", readBuffer.get());
	*/
	return 0;
}
