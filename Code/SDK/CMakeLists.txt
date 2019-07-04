if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
        set(ARCH x64)
else()
        set(ARCH x86)
endif()

# Add Assimp as IMPORTED
add_library(Assimp SHARED IMPORTED GLOBAL)
set_target_properties(Assimp PROPERTIES
        IMPORTED_LOCATION             "${CMAKE_CURRENT_LIST_DIR}/Assimp/bin/${ARCH}/assimp-vc140-mt.dll"
        IMPORTED_IMPLIB               "${CMAKE_CURRENT_LIST_DIR}/Assimp/lib/${ARCH}/assimp-vc140-mt.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/Assimp/include" 
        INTERFACE_COMPILE_DEFINITIONS ""
)
list(APPEND ZHU_SDK_DLLS "${CMAKE_CURRENT_LIST_DIR}/Assimp/bin/${ARCH}/assimp-vc140-mt.dll")


# Add Assimp as IMPORTED
add_library(SDL2 SHARED IMPORTED GLOBAL)
set_target_properties(SDL2 PROPERTIES
        IMPORTED_LOCATION             "${CMAKE_CURRENT_LIST_DIR}/SDL2/bin/${ARCH}/SDL2.dll"
        IMPORTED_IMPLIB               "${CMAKE_CURRENT_LIST_DIR}/SDL2/lib/${ARCH}/SDL2.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/SDL2/include" 
        INTERFACE_COMPILE_DEFINITIONS ""
)
list(APPEND ZHU_SDK_DLLS "${CMAKE_CURRENT_LIST_DIR}/SDL2/bin/${ARCH}/SDL2.dll")

# Add Google Test as IMPORTED
add_library(GoogleTest     STATIC IMPORTED GLOBAL)
add_library(GoogleTestMock STATIC IMPORTED GLOBAL)
set_target_properties(GoogleTest PROPERTIES
        IMPORTED_LOCATION             "${CMAKE_CURRENT_LIST_DIR}/GoogleTest/lib/${ARCH}/gtest.lib"
        IMPORTED_LOCATION_DEBUG       "${CMAKE_CURRENT_LIST_DIR}/GoogleTest/lib/${ARCH}/gtestd.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/GoogleTest/include" 
        INTERFACE_COMPILE_DEFINITIONS ""
)
set_target_properties(GoogleTestMock PROPERTIES
        IMPORTED_LOCATION             "${CMAKE_CURRENT_LIST_DIR}/GoogleTest/lib/${ARCH}/gmock.lib"
        IMPORTED_LOCATION_DEBUG       "${CMAKE_CURRENT_LIST_DIR}/GoogleTest/lib/${ARCH}/gmockd.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/GoogleTest/include" 
        INTERFACE_COMPILE_DEFINITIONS ""
)


# Add Eigen as INTERFACE
add_library(Eigen INTERFACE IMPORTED GLOBAL)
set_target_properties(Eigen PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/Eigen/include"
        INTERFACE_COMPILE_DEFINITIONS ""
)


# Add JSON as INTERFACE
add_library(JSON INTERFACE IMPORTED GLOBAL)
set_target_properties(JSON PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/JSON/include"
        INTERFACE_COMPILE_DEFINITIONS ""
)

set(ZHU_SDK_DLLS ${ZHU_SDK_DLLS} PARENT_SCOPE)


