# Add Assimp as IMPORTED
add_library(Assimp SHARED IMPORTED GLOBAL)
set(AssimpDir "${CMAKE_CURRENT_LIST_DIR}/Library/Assimp")
set_target_properties(Assimp PROPERTIES
    IMPORTED_LOCATION             "${AssimpDir}/bin/assimp-vc140-mt.dll"
    IMPORTED_IMPLIB               "${AssimpDir}/lib/assimp-vc140-mt.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${AssimpDir}/include" 
    INTERFACE_COMPILE_DEFINITIONS ""
)

# Add SDL2 as IMPORTED
add_library(SDL2 SHARED IMPORTED GLOBAL)
set(SDL2Dir "${CMAKE_CURRENT_LIST_DIR}/Library/SDL2")
set_target_properties(SDL2 PROPERTIES
    IMPORTED_LOCATION             "${SDL2Dir}/bin/SDL2.dll"
    IMPORTED_IMPLIB               "${SDL2Dir}/lib/SDL2.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${SDL2Dir}/include" 
    INTERFACE_COMPILE_DEFINITIONS ""
)

# Add TinyXML2 as IMPORTED
add_library(TinyXML2 STATIC IMPORTED GLOBAL)
set(TinyXML2Dir "${CMAKE_CURRENT_LIST_DIR}/Library/TinyXML2")
set_target_properties(TinyXML2 PROPERTIES
    IMPORTED_LOCATION             "${TinyXML2Dir}/lib/tinyxml2.lib"
    IMPORTED_LOCATION_DEBUG       "${TinyXML2Dir}/lib/tinyxml2d.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${TinyXML2Dir}/include" 
    INTERFACE_COMPILE_DEFINITIONS ""
)

# Add Google Test as IMPORTED
add_library(GoogleTest STATIC IMPORTED GLOBAL)
add_library(GoogleMock STATIC IMPORTED GLOBAL)
set(GoogleTestDir "${CMAKE_CURRENT_LIST_DIR}/Library/GoogleTest")
set_target_properties(GoogleTest PROPERTIES
    IMPORTED_LOCATION             "${GoogleTestDir}/lib/gtest.lib"
    IMPORTED_LOCATION_DEBUG       "${GoogleTestDir}/lib/gtestd.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${GoogleTestDir}/include" 
    INTERFACE_COMPILE_DEFINITIONS ""
)
set_target_properties(GoogleMock PROPERTIES
    IMPORTED_LOCATION             "${GoogleTestDir}/lib/gmock.lib"
    IMPORTED_LOCATION_DEBUG       "${GoogleTestDir}/lib/gmockd.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${GoogleTestDir}/include" 
    INTERFACE_COMPILE_DEFINITIONS ""
)

# Add JSON as INTERFACE
add_library(JSON INTERFACE IMPORTED GLOBAL)
set(JSONDir "${CMAKE_CURRENT_LIST_DIR}/Code/JSON")
set_target_properties(JSON PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${JSONDir}/include"
    INTERFACE_COMPILE_DEFINITIONS ""
)

# Add Eigen as INTERFACE
add_library(Eigen INTERFACE IMPORTED GLOBAL)
set(EigenDir "${CMAKE_CURRENT_LIST_DIR}/Code/Eigen")
set_target_properties(Eigen PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${EigenDir}/include"
    INTERFACE_COMPILE_DEFINITIONS ""
)
