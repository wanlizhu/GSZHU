file(GLOB SRC_COMMON
    "*.cpp"
    "*.h"
)

file(GLOB SRC_INTERFACE
    "../Interface/*.h"
)

file(GLOB SRC_UTIL
    "../Util/*.cpp"
    "../Util/*.h"
)

file(GLOB SRC_MATH
    "../Math/*.h"
)

source_group(Interface FILES ${SRC_INTERFACE})
source_group(Uitl FILES ${SRC_UTIL})
source_group(Math FILES ${SRC_MATH})

include_directories("${PROJECT_SOURCE_DIR}/Thirdparts/Vulkan/Include")
include_directories("${PROJECT_SOURCE_DIR}/Thirdparts/SPIRV-Cross/Include")
include_directories("${PROJECT_SOURCE_DIR}/Thirdparts/VMA/")

add_library(
    Common
    ${SRC_COMMON}
    ${SRC_UTIL}
    ${SRC_MATH}
    ${SRC_INTERFACE}
)

set_target_properties(
    Common
    PROPERTIES
    FOLDER ${FOLDER_ENGINE}
)