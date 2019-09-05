cmake_minimum_required(VERSION 2.8.11)

# The INTERFACE, PUBLIC and PRIVATE keywords are required to specify the scope of the following arguments. 
# PRIVATE and PUBLIC items will populate the INCLUDE_DIRECTORIES property of <target>. 
# PUBLIC and INTERFACE items will populate the INTERFACE_INCLUDE_DIRECTORIES property of <target>. 
# (IMPORTED targets only support INTERFACE items.) 

# Eigen
add_library(EIGEN INTERFACE IMPORTED GLOBAL)
target_include_directories(EIGEN INTERFACE Eigen/include)
target_compile_definitions(EIGEN INTERFACE )

set(ExternalImportedTargetList
	EIGEN
	GLFW
	STB
	GLOG
	PARENT_SCOPE)