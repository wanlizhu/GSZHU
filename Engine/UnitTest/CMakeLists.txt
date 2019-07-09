file(GLOB_RECURSE SOURCE_LIST "*.h" "*.cpp")

add_executable(ZHUENGINE_UnitTest ${SOURCE_LIST})

target_include_directories(ZHUENGINE_UnitTest
PRIVATE
    "${ZHU_ENGINE_DIR}/Source"
)

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