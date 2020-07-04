// CPakReader.cpp : Defines the entry point for the application.
//

#include "CPakReader.h"
#include "src/CFileStream.h"
#include "src/CFileTree.h"
#include "src/CIndex.h"
#include "src/CPackage.h"
#include "src/EAssetType.h"
#include "src/HStringHash.h"
#include "src/Objects/FPakInfo.h"

#include <iomanip>
#include <fstream>
#include <variant>

using namespace std;

int main()
{
	CFileStream stream("J:\\Programs\\FN\\cache\\workaround\\FortniteGame\\Content\\Paks\\pakChunkEarly-WindowsClient.pak");
	stream.seek(-221, CStream::End);
	FPakInfo info;
	stream >> info;
	
	//CIndex(5);
	cout << "Hello CMake." << endl;
	return 0;
}
