cmake_minimum_required(VERSION 3.11)

project(GraphicsEngine)

set(ENABLE_VULKAN ON  CACHE BOOL "")
set(ENABLE_D3D12 OFF CACHE BOOL "")
set(ENABLE_PCH OFF CACHE BOOL "")
set(PRECOMPILE_SHADER ON CACHE BOOL "")
set(ANDROID OFF CACHE BOOL "")

add_subdirectory(Engine)
add_subdirectory(Samples)