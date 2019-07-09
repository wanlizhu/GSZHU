file(GLOB_RECURSE SOURCE_LIST "Private/*.cpp" "Private/*.c")
file(GLOB_RECURSE HEADER_LIST "Public/ZHUENGINE/*.hpp" "Public/ZHUENGINE/*.h")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADER_LIST})

# Add headers
target_sources(ZHUENGINE PRIVATE ${HEADER_LIST})

# Add sources
foreach(SRC IN LISTS SOURCE_LIST)
        if(SRC MATCHES ".*[_Win32|_Linux|_Darwin]\.[c|cpp]$")
                target_sources(ZHUENGINE PRIVATE 
                        "$<$<PLATFORM_ID:Windows>:${SRC}>"
                        "$<$<PLATFORM_ID:Linux>:${SRC}>"
                        "$<$<PLATFORM_ID:Darwin>:${SRC}>"
                )
        else()
            target_sources(ZHUENGINE PRIVATE "${SRC}")
        endif()
endforeach()

include(PrecompiledHeader)
add_precompiled_header(ZHUENGINE 
        ZHUENGINE_PCH.h 
SOURCE_CXX 
        ZHUENGINE_PCH.cpp
FORCEINCLUDE
)


