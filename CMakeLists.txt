cmake_minimum_required (VERSION 3.8)

project (CPakReader)

add_executable (CPakReader "CPakReader.cpp"  "src/CIndex.h" "src/CPackage.h" "src/EAssetType.h" "src/CIndex.cpp" "src/HStringHash.h" "src/CFileTree.h" "src/EErrorCode.h" "src/Objects/FAESKey.h" "src/Objects/EBulkDataFlags.h" "src/Objects/ECompressionFlags.h" "src/Objects/ECurveTableMode.h" "src/Objects/EDateTimeStyle.h" "src/Objects/EObjectFlags.h" "src/Objects/EPackageFlags.h" "src/Objects/EPakVersion.h" "src/Objects/EPixelFormat.h" "src/Objects/ETextFlag.h" "src/Objects/ETextHistoryType.h" "src/Objects/FPakInfo.h" "src/Objects/FSHAHash.h" "src/CStream.h" "src/CFileStream.h" "src/CMemoryStream.h" "src/HAES.h"  "src/CPackageFile.h" "src/Objects/FPakEntry.h" "src/Objects/FPakCompressedBlock.h" "src/FPakFile.h"  )

set_property(TARGET CPakReader PROPERTY CXX_STANDARD 20)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Release")
   target_compile_options(CPakReader PRIVATE /Zi)
    set_target_properties(CPakReader PROPERTIES
        LINK_FLAGS "/INCREMENTAL:NO /DEBUG /OPT:REF /OPT:ICF"
        COMPILE_PDB_NAME CPakReader 
        COMPILE_PDB_OUTPUT_DIR ${CMAKE_BINARY_DIR}
    )
endif()