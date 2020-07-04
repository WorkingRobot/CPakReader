cmake_minimum_required (VERSION 3.8)

project (CPakReader)

add_executable (CPakReader "CPakReader.cpp"  "src/CIndex.h" "src/CPackage.h" "src/EAssetType.h" "src/CIndex.cpp" "src/HStringHash.h" "src/CFileTree.h" "src/EErrorCode.h" "src/Objects/FAESKey.h" "src/Objects/EBulkDataFlags.h" "src/Objects/ECompressionFlags.h" "src/Objects/ECurveTableMode.h" "src/Objects/EDateTimeStyle.h" "src/Objects/EObjectFlags.h" "src/Objects/EPackageFlags.h" "src/Objects/EPakVersion.h" "src/Objects/EPixelFormat.h" "src/Objects/ETextFlag.h" "src/Objects/ETextHistoryType.h" "src/Readers/RPakReader.h" "src/Readers/RPakReader.cpp" "src/Objects/FPakInfo.h" "src/Objects/FSHAHash.h" "src/CStream.h" "src/CFileStream.h")

set_property(TARGET CPakReader PROPERTY CXX_STANDARD 20)