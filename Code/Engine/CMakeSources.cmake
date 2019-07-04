set(ZHU_HDR_FILES_WITH_ARGS
PUBLIC
        Common/Config.h
        Common/String.h
        Common/BasicTypes.h
        Common/ConstExpr.h
        Common/EnumClass.h
        Common/IEvent.h
        Common/NonCopyable.h
        Common/Object.h
        Common/ObjectFactory.h
        Common/Signal.h
        Common/ThreadPool.h

PRIVATE
        ZHUENGINE_PCH.h
        Common/Platform/OS.h
        Common/Log.h
        Common/WindowsFiles.h
)

set(ZHU_SRC_FILES_WITH_ARGS
ALL
        ZHUENGINE_PCH.cpp
        Common/Log.cpp
        Common/Object.cpp
        Common/String.cpp
        Common/ThreadPool.cpp
WINDOWS
        Common/Platform/OS_Win32.cpp
)






source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ZHU_HDR_FILES_WITH_ARGS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ZHU_SRC_FILES_WITH_ARGS})

# Add headers
set(MULTI_VALUE_ARGS PUBLIC PRIVATE)
cmake_parse_arguments(ZHU_HDR "" "" "${MULTI_VALUE_ARGS}" ${ZHU_HDR_FILES_WITH_ARGS})

foreach(HDR IN LISTS ZHU_HDR_PUBLIC ZHU_HDR_PRIVATE)
        if(HDR IN_LIST ZHU_HDR_PUBLIC)
                target_sources(ZHUENGINE PRIVATE  
                        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${HDR}>
                        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${HDR}>
                )
        elseif(HDR IN_LIST ZHU_HDR_PRIVATE)
                target_sources(ZHUENGINE PRIVATE 
                        "${CMAKE_CURRENT_SOURCE_DIR}/${HDR}"
                )
        else()
                message(FATAL_ERROR "!!! Invalid header file option (it's neither PUBLIC nor PRIVATE)")
        endif()
endforeach()

# Add sources
set(MULTI_VALUE_ARGS WINDOWS LINUX MACOS ALL)
cmake_parse_arguments(ZHU_SRC "" "" "${MULTI_VALUE_ARGS}" ${ZHU_SRC_FILES_WITH_ARGS})

foreach(SRC IN LISTS ZHU_SRC_WINDOWS ZHU_SRC_LINUX ZHU_SRC_MACOS ZHU_SRC_ALL)
        if(SRC IN_LIST ZHU_SRC_WINDOWS)
                target_sources(ZHUENGINE PRIVATE
                        "$<$<PLATFORM_ID:Windows>:${CMAKE_CURRENT_SOURCE_DIR}/${SRC}>"
                )
        elseif(SRC IN_LIST ZHU_SRC_LINUX)
                target_sources(ZHUENGINE PRIVATE
                        "$<$<PLATFORM_ID:Linux>:${CMAKE_CURRENT_SOURCE_DIR}/${SRC}>"
                )
        elseif(SRC IN_LIST ZHU_SRC_MACOS)
                target_sources(ZHUENGINE PRIVATE
                        "$<$<PLATFORM_ID:Darwin>:${CMAKE_CURRENT_SOURCE_DIR}/${SRC}>"
                )
        elseif(SRC IN_LIST ZHU_SRC_ALL)
                target_sources(ZHUENGINE PRIVATE
                        "${CMAKE_CURRENT_SOURCE_DIR}/${SRC}"
                )
        else()
                message(FATAL_ERROR "Invalid source file platform option (it's neither WINDOWS, LINUX, MACOS nor ALL)")
        endif()
endforeach()

include(PrecompiledHeader)
add_precompiled_header(ZHUENGINE 
        ZHUENGINE_PCH.h 
SOURCE_CXX 
        ZHUENGINE_PCH.cpp
FORCEINCLUDE
)


