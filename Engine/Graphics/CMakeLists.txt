file(GLOB SRC_DEVICE
    "Device/*.cpp"
    "Device/*.h"
)

file(GLOB SRC_D3D11
    "Device/D3D11/*.cpp"
    "Device/D3D11/*.h"
)

file(GLOB SRC_D3D12
    "Device/D3D12/*.cpp"
    "Device/D3D12/*.h"
)

file(GLOB SRC_VK
	"Device/Vulkan/*.cpp"
    "Device/Vulkan/*.h"
)

file(GLOB SRC_RENDERER
    "Renderer/*.cpp"
    "Renderer/*.h"
    "Renderer/Geometry/*.cpp"
    "Renderer/Geometry/*.h"
)

file(GLOB SRC_MATH
    "../Math/*.h"
)

add_library(
    Graphics
    ${SRC_DEVICE}
    ${SRC_D3D11}
    ${SRC_D3D12}
	${SRC_VK}
    ${SRC_RENDERER}
    ${SRC_MATH}
)

include_directories("${PROJECT_SOURCE_DIR}/Thirdparts/Effect11/Include")
include_directories("${PROJECT_SOURCE_DIR}/Thirdparts/D3DX12/Include")
include_directories("${PROJECT_SOURCE_DIR}/Thirdparts/Vulkan/Include")
include_directories("${PROJECT_SOURCE_DIR}/Thirdparts/SPIRV-Cross/Include")
include_directories("${PROJECT_SOURCE_DIR}/Thirdparts/VMA/")

find_library(Effect11_LIB Effects11.lib HINTS ${PROJECT_SOURCE_DIR}/Thirdparts/Effect11/Lib)
find_library(VK_LIB vulkan-1.lib HINTS ${PROJECT_SOURCE_DIR}/Thirdparts/Vulkan/Lib)
find_library(SPIRV_CROSS_UTIL_LIB_DEBUG spirv-cross-util.lib HINTS ${PROJECT_SOURCE_DIR}/Thirdparts/SPIRV-Cross/Lib)
find_library(SPIRV_CROSS_CORE_LIB_DEBUG spirv-cross-core.lib HINTS ${PROJECT_SOURCE_DIR}/Thirdparts/SPIRV-Cross/Lib)
find_library(SPIRV_CROSS_GLSL_LIB_DEBUG spirv-cross-glsl.lib HINTS ${PROJECT_SOURCE_DIR}/Thirdparts/SPIRV-Cross/Lib)
find_library(SPIRV_CROSS_REFLECT_LIB_DEBUG spirv-cross-reflect.lib HINTS ${PROJECT_SOURCE_DIR}/Thirdparts/SPIRV-Cross/Lib)
find_library(SPIRV_CROSS_UTIL_LIB spirv-cross-util.lib HINTS ${PROJECT_SOURCE_DIR}/Thirdparts/SPIRV-Cross/Lib/Release)
find_library(SPIRV_CROSS_CORE_LIB spirv-cross-core.lib HINTS ${PROJECT_SOURCE_DIR}/Thirdparts/SPIRV-Cross/Lib/Release)
find_library(SPIRV_CROSS_GLSL_LIB spirv-cross-glsl.lib HINTS ${PROJECT_SOURCE_DIR}/Thirdparts/SPIRV-Cross/Lib/Release)
find_library(SPIRV_CROSS_REFLECT_LIB spirv-cross-reflect.lib HINTS ${PROJECT_SOURCE_DIR}/Thirdparts/SPIRV-Cross/Lib/Release)

target_link_libraries(
    Graphics
    ${Effect11_LIB}
	${VK_LIB}
	debug ${SPIRV_CROSS_UTIL_LIB_DEBUG}
	debug ${SPIRV_CROSS_CORE_LIB_DEBUG}
	debug ${SPIRV_CROSS_GLSL_LIB_DEBUG}
	debug ${SPIRV_CROSS_REFLECT_LIB_DEBUG}
	optimized ${SPIRV_CROSS_UTIL_LIB}
	optimized ${SPIRV_CROSS_CORE_LIB}
	optimized ${SPIRV_CROSS_GLSL_LIB}
	optimized ${SPIRV_CROSS_REFLECT_LIB}
    d3d11.lib
    d3d12.lib
    dxgi.lib
    d3dcompiler.lib
    dxguid.lib
)

source_group(Device FILES ${SRC_DEVICE})
source_group(Device\\D3D11 FILES ${SRC_D3D11})
source_group(Device\\D3D12 FILES ${SRC_D3D12})
source_group(Device\\Vulkan FILES ${SRC_VK})
source_group(Renderer FILES ${SRC_RENDERER})
source_group(Math FILES ${SRC_MATH})

set_target_properties(
    Graphics
    PROPERTIES
    FOLDER ${FOLDER_ENGINE}
)