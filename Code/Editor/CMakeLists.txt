cmake_minimum_required(VERSION 3.6.2)

# Build shared library if variable "BUILD_SHARED_LIBS" is ON
add_executable(LynxEditor main.cpp)
target_link_libraries(LynxEditor LynxEngine)
