set(ZHU_HDR_FILES_PUBLIC
        Common/BasicTypes.h
        Common/Config.h
        Common/ConstExpr.h
        Common/EnumClass.h
        Common/IEvent.h
        Common/NonCopyable.h
        Common/Object.h
        Common/ObjectFactory.h
        Common/Signal.h
        Common/ThreadPool.h
        Common/WindowsFiles.h
)


set(ZHU_HDR_FILES_PRIVATE
        ZHUENGINE_PCH.h
        Common/Platform/OS.h
        Common/Log.h
        Common/String.h
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
foreach(HDR IN LISTS ZHU_HDR_FILES_PUBLIC)
        target_sources(ZHUENGINE PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${HDR}>
                $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${HDR}>
        )
endforeach()

# Add private headers
foreach(HDR IN LISTS ZHU_HDR_FILES_PRIVATE)
        target_sources(ZHUENGINE PRIVATE
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${HDR}>
        )
endforeach()

# Add private source files
foreach(SRC IN LISTS ZHU_SRC_FILES)
        target_sources(ZHUENGINE PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/${SRC}
		)
endforeach()

include(PrecompiledHeader)
#add_precompiled_header(ZHUENGINE 
#        ZHUENGINE_PCH.h 
#        SOURCE_CXX 
#        ZHUENGINE_PCH.cpp
#        FORCEINCLUDE
#)

