cmake_minimum_required(VERSION 3.11)

# Add module path and include
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/develop/cmake")
include(CMakeAuxiliary)
include(GraphicsEngineAuxiliary)

# User customized settings
set(GE_CONFIG         "DEBUG"   CACHE STRING "")
set(GE_LINKAGE        "SHARED"  CACHE STRING "")
set(GE_ISET           "X64"     CACHE STRING "")
set(GE_PLATFORM       "WINDOWS" CACHE STRING "")
set(GE_BUILD_SAMPLES  TRUE      CACHE BOOL   "")
set(GE_BUILD_TEST     TRUE      CACHE BOOL   "")

# Construct the default packages directory
set(_triplets "${GE_ISET}-${GE_PLATFORM}-static")
string(TOLOWER "${_triplets}" _triplets)
set(GE_PACKAGES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/packages/${_triplets}" CACHE STRING "")

# Setup project
project(GraphicsEngine)
cm_setup_project(GraphicsEngine)

# Add subdirectory
add_subdirectory(runtime)
add_subdirectory(samples)
if (GE_BUILD_TEST)
    add_subdirectory(tests)
endif()
