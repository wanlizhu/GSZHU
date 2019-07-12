file(GLOB_RECURSE SOURCE_LIST "*.h" "*.cpp")

add_executable(Engine_UnitTest ${SOURCE_LIST})

target_include_directories(Engine_UnitTest
PRIVATE
    "../Engine"
)

target_link_libraries(Engine_UnitTest
    Engine
    GoogleTest
    GoogleMock
)

set_target_properties(Engine_UnitTest PROPERTIES
	FOLDER "ENGINE"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)