file(GLOB_RECURSE PublicSourceFiles  "${CMAKE_CURRENT_SOURCE_DIR}/include/*")
file(GLOB_RECURSE PrivateSourceFiles "${CMAKE_CURRENT_SOURCE_DIR}/source/*")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${PublicSourceFiles})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/source"  PREFIX "Source Files" FILES ${PrivateSourceFiles})

set(SourceFiles ${PublicSourceFiles} ${PrivateSourceFiles})