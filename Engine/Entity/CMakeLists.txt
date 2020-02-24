file(GLOB SRC_ENTITY
    "*.cpp"
    "*.h"
)

file(GLOB SRC_COMPONENT
    "Component/*.cpp"
    "Component/*.h"
)

file(GLOB SRC_MESH
    "Mesh/*.cpp"
    "Mesh/*.h"
)

file(GLOB SRC_SCENE
    "Scene/*.cpp"
    "Scene/*.h"
)

file(GLOB SRC_GOBFILELOADER
    "GobFileLoader/*.cpp"
    "GobFileLoader/*.h"
)

source_group(Component FILES ${SRC_COMPONENT})
source_group(Scene FILES ${SRC_SCENE})

add_library(
    Entity
    ${SRC_ENTITY}
	${SRC_COMPONENT}
    ${SRC_MESH}
    ${SRC_SCENE}
    ${SRC_SYSTEM}
    ${SRC_GOBFILELOADER}
)

include_directories("${PROJECT_SOURCE_DIR}/Thirdparts/glTF2-loader/Include")
find_library(GLTF2_LOADER_LIB_DEBUG gltf2-loader-d.lib HINTS ${PROJECT_SOURCE_DIR}/Thirdparts/glTF2-loader/Lib/Debug)
find_library(GLTF2_LOADER_LIB gltf2-loader.lib HINTS ${PROJECT_SOURCE_DIR}/Thirdparts/glTF2-loader/Lib/Release)
target_link_libraries(
    Entity
    optimized ${GLTF2_LOADER_LIB}
	debug ${GLTF2_LOADER_LIB_DEBUG}
)

set_target_properties(
    Entity
    PROPERTIES
    FOLDER ${FOLDER_ENGINE}
)