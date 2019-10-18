#cm_use_vulkan()
function(cm_use_vulkan)
    find_package(Vulkan REQUIRED)
    cm_link_libraries(Vulkan::Vulkan)
endfunction()

# cm_use_eigen3()
# Eigen3 is a header-only library
function(cm_use_eigen3)
    if (VCPKG_TOOLCHAIN)
        find_package(Eigen3 CONFIG REQUIRED)
    else()
        include("${GE_PACKAGES_DIR}/share/eigen3/Eigen3Config.cmake")
    endif()

    cm_link_libraries(Eigen3::Eigen)
endfunction()

# cm_use_nlohmann_json()
# nlohmann_json is a header-only library
function(cm_use_nlohmann_json)
    if (VCPKG_TOOLCHAIN)
        find_package(nlohmann_json CONFIG REQUIRED)
    else()
        include("${GE_PACKAGES_DIR}/share/nlohmann_json/nlohmann_jsonConfig.cmake")
    endif()

    cm_link_libraries(nlohmann_json::nlohmann_json)
endfunction()

# cm_use_tinygltf()
function(cm_use_tinygltf)
    # there is no CMake targets provided by VCPKG_TOOLCHAIN
    add_library(tinygltf::tinygltf INTERFACE IMPORTED GLOBAL)
    target_include_directories(tinygltf::tinygltf INTERFACE "${GE_PACKAGES_DIR}/include")
    foreach(file "stb_image.h" "nlohmann/json.hpp")
        if (NOT EXISTS "${GE_PACKAGES_DIR}/include/${file}")
            message(FATAL_ERROR "header file dependency(${file}) does not exist")
        endif()
    endforeach()

    cm_link_libraries(tinygltf::tinygltf)
endfunction()

# cm_use_glfw3()
function(cm_use_glfw3)
    if (VCPKG_TOOLCHAIN)
        find_package(glfw3 CONFIG REQUIRED)
    else()
        include("${GE_PACKAGES_DIR}/share/glfw3/glfw3Config.cmake")
    endif()

    cm_link_libraries(glfw)
endfunction()

# cm_use_imgui()
function(cm_use_imgui)
    if (VCPKG_TOOLCHAIN)
        find_package(imgui CONFIG REQUIRED)
    else()
        include("${GE_PACKAGES_DIR}/share/imgui/imguiConfig.cmake")
    endif()

    cm_link_libraries(imgui::imgui)
endfunction()

# cm_use_tinyobjloader()
function(cm_use_tinyobjloader)
    if (VCPKG_TOOLCHAIN)
        find_package(tinyobjloader CONFIG REQUIRED)
    else()
        include("${GE_PACKAGES_DIR}/share/tinyobjloader/tinyobjloader-config.cmake")
    endif()

    cm_link_libraries(tinyobjloader::tinyobjloader)
endfunction()

# cm_use_gtest()
function(cm_use_gtest)
    if (VCPKG_TOOLCHAIN)
        find_package(GTest CONFIG REQUIRED)
    else()
        include("${GE_PACKAGES_DIR}/share/gtest/GTestConfig.cmake")
    endif()

    cm_link_libraries(GTest::gtest GTest::gmock)
endfunction()