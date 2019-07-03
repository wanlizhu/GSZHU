cmake_minimum_required(VERSION 3.6.2)

add_library(Assimp SHARED IMPORTED GLOBAL)
set_property(TARGET Assimp
        PROPERTY IMPORTED_LOCATION
        "${CMAKE_CURRENT_LIST_DIR}/bin/x64/assimp-vc140-mt.dll")
set_property(TARGET Assimp
        PROPERTY IMPORTED_IMPLIB
        "${CMAKE_CURRENT_LIST_DIR}/lib/x64/assimp-vc140-mt.lib")
set_property(TARGET Assimp 
        APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
        "${CMAKE_CURRENT_LIST_DIR}/include")
