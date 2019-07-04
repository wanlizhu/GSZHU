file(GLOB_RECURSE SOURCES "*.cpp" "*.h")

add_executable(ZHUENGINE_UnitTest
        ${SOURCES} 
)
target_link_libraries(ZHUENGINE_UnitTest PRIVATE
        ZHUENGINE 
        GoogleTest 
        GoogleTestMock
)
set_target_properties(ZHUENGINE_UnitTest PROPERTIES
		FOLDER "UnitTest"
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
target_compile_definitions(ZHUENGINE PRIVATE
        ""
)