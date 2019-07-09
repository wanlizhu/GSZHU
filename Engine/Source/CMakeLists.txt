file(GLOB_RECURSE SOURCE_LIST "*.cpp" "*.c")
file(GLOB_RECURSE HEADER_LIST "*.hpp" "*.h")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADER_LIST})

# Add sources to target
target_sources(ZHUENGINE PRIVATE ${HEADER_LIST})
target_sources(ZHUENGINE PRIVATE ${SOURCE_LIST})

include(PrecompiledHeader)
add_precompiled_header(ZHUENGINE 
        ZHUENGINE_PCH.h 
SOURCE_CXX 
        ZHUENGINE_PCH.cpp
FORCEINCLUDE
)


