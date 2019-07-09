add_executable(GTestRunner IMPORTED GLOBAL)
set_property(TARGET GTestRunner 
    PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/GTestRunner/bin/gtest-runner.exe"
)

