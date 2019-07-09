add_subdirectory(ThirdParty)
add_subdirectory(Runtime)

if(BUILD_EDITOR)
    add_subdirectory(Editor)
endif()
if(BUILD_TESTS)
    add_subdirectory(UnitTest)
endif()