# Add Assimp as IMPORTED
add_library(Assimp SHARED IMPORTED GLOBAL)
set_target_properties(Assimp PROPERTIES
    IMPORTED_LOCATION             "${CMAKE_CURRENT_LIST_DIR}/Assimp/bin/x64/assimp-vc140-mt.dll"
    IMPORTED_IMPLIB               "${CMAKE_CURRENT_LIST_DIR}/Assimp/lib/x64/assimp-vc140-mt.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/Assimp/include" 
    INTERFACE_COMPILE_DEFINITIONS ""
)

# Add Assimp as IMPORTED
add_library(SDL2 SHARED IMPORTED GLOBAL)
set_target_properties(SDL2 PROPERTIES
    IMPORTED_LOCATION             "${CMAKE_CURRENT_LIST_DIR}/SDL2/bin/x64/SDL2.dll"
    IMPORTED_IMPLIB               "${CMAKE_CURRENT_LIST_DIR}/SDL2/lib/x64/SDL2.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/SDL2/include" 
    INTERFACE_COMPILE_DEFINITIONS ""
)

# Add Google Test as IMPORTED
add_library(GoogleTest STATIC IMPORTED GLOBAL)
add_library(GoogleMock STATIC IMPORTED GLOBAL)
set_target_properties(GoogleTest PROPERTIES
    IMPORTED_LOCATION             "${CMAKE_CURRENT_LIST_DIR}/GoogleTest/lib/x64/gtest.lib"
    IMPORTED_LOCATION_DEBUG       "${CMAKE_CURRENT_LIST_DIR}/GoogleTest/lib/x64/gtestd.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/GoogleTest/include" 
    INTERFACE_COMPILE_DEFINITIONS ""
)
set_target_properties(GoogleMock PROPERTIES
    IMPORTED_LOCATION             "${CMAKE_CURRENT_LIST_DIR}/GoogleTest/lib/x64/gmock.lib"
    IMPORTED_LOCATION_DEBUG       "${CMAKE_CURRENT_LIST_DIR}/GoogleTest/lib/x64/gmockd.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/GoogleTest/include" 
    INTERFACE_COMPILE_DEFINITIONS ""
)
