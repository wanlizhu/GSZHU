cmake_minimum_required(VERSION 2.8.11)

# The INTERFACE, PUBLIC and PRIVATE keywords are required to specify the scope of the following arguments. 
# PRIVATE and PUBLIC items will populate the INCLUDE_DIRECTORIES property of <target>. 
# PUBLIC and INTERFACE items will populate the INTERFACE_INCLUDE_DIRECTORIES property of <target>. 
# (IMPORTED targets only support INTERFACE items.) 

# Eigen
add_library(External::EIGEN INTERFACE IMPORTED GLOBAL)
target_include_directories(External::EIGEN INTERFACE "Eigen/include")

# GLFW
add_library(External::GLFW STATIC IMPORTED GLOBAL)
target_include_directories(External::GLFW INTERFACE "GLFW/include")
set_target_properties(External::GLFW PROPERTIES 
    IMPORTED_LOCATION_Debug   "GLFW/lib/glfw3d.lib"
    IMPORTED_LOCATION_Release "GLFW/lib/glfw3.lib")
    
#glog
add_library(External::GLOG STATIC IMPORTED GLOBAL)
target_include_directories(External::GLOG INTERFACE "glog/include")
set_target_properties(External::GLOG PROPERTIES 
    IMPORTED_LOCATION_Debug   "glog/lib/glogd.lib"
    IMPORTED_LOCATION_Release "glog/lib/glog.lib")
    
# gtest
add_library(External::GTEST STATIC IMPORTED GLOBAL)
target_include_directories(External::GTEST INTERFACE "gtest/include")
set_target_properties(External::GTEST PROPERTIES 
    IMPORTED_LOCATION_Debug   "gtest/lib/gtestd.lib"
    IMPORTED_LOCATION_Release "gtest/lib/gtest.lib")
    
# stb
add_library(External::STB STATIC IMPORTED GLOBAL)
target_include_directories(External::STB INTERFACE "stb/include")
set_target_properties(External::STB PROPERTIES 
    IMPORTED_LOCATION_Debug   "stb/lib/stbd.lib"
    IMPORTED_LOCATION_Release "stb/lib/stb.lib")
