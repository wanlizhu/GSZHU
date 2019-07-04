set(ZHU_HDR_FILES
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

set(ZHU_SRC_FILES
        ZHUENGINE_PCH.cpp
        $<$<PLATFORM_ID:Windows>:Common/Platform/OS_Win32.cpp>
        Common/Log.cpp
        Common/Object.cpp
        Common/String.cpp
        Common/ThreadPool.cpp
)





# Add public headers (to be installed)
set(IS_PUBLIC_HDR ON)
foreach(HDR IN LISTS ZHU_HDR_FILES)
        if (HDR STREQUAL "PUBLIC")
                set(IS_PUBLIC_HDR ON)
        elseif(HDR STREQUAL "PRIVATE")
                set(IS_PUBLIC_HDR OFF)
        else()
                if(${IS_PUBLIC_HDR})
                        target_sources(ZHUENGINE PUBLIC
                        ${CMAKE_CURRENT_SOURCE_DIR}/${HDR}
                                #$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${HDR}>
                                #$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${HDR}>
                        )
                else()
                        target_sources(ZHUENGINE PRIVATE
                        ${CMAKE_CURRENT_SOURCE_DIR}/${HDR}
                                #${CMAKE_CURRENT_SOURCE_DIR}/${HDR}
                        )
                endif()
        endif()
endforeach()

# Add private source files
foreach(SRC IN LISTS ZHU_SRC_FILES)
        target_sources(ZHUENGINE PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/${SRC}
                #${CMAKE_CURRENT_SOURCE_DIR}/${SRC}
        )
endforeach()

include(PrecompiledHeader)
add_precompiled_header(ZHUENGINE 
        ZHUENGINE_PCH.h 
SOURCE_CXX 
        ZHUENGINE_PCH.cpp
FORCEINCLUDE
)

