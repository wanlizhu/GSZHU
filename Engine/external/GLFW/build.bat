@echo off

mkdir build
cmake -G "Visual Studio 16 2019" -S "." -B "build" -DGLFW_BUILD_DOCS=FALSE -DGLFW_BUILD_EXAMPLES=FALSE -DGLFW_BUILD_TESTS=FALSE -DGLFW_INSTALL=FALSE   

cmake --build build