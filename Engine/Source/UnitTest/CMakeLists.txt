file(GLOB_RECURSE SOURCE_LIST "*.h" "*.cpp")

add_executable(ZHUENGINE_UnitTest ${SOURCE_LIST})
include_directories("${ZHU_ENGINE_DIR}/Source/Runtime/Private")
target_link_libraries(ZHUENGINE_UnitTest
    ZHUENGINE
    GoogleTest
    GoogleMock
)

set_target_properties(ZHUENGINE_UnitTest PROPERTIES
	FOLDER "ZHUENGINE"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)