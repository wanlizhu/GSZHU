if (WIN32 AND WANLI_BUILD_WINDOWS)
    add_subdirectory(Windows)
endif()

if (WANLI_BUILD_ANDROID)
    add_subdirectory(Android)
endif()