cmake_minimum_required(VERSION 2.8.11)

set(DOT ${CMAKE_CURRENT_SOURCE_DIR})

# Cross-Platform Files
file(GLOB_RECURSE CorePublic "core/public/*")
file(GLOB_RECURSE GeometryPublic "geometry/public/*")
file(GLOB_RECURSE RayTracingPublic "raytracing/public/*")
file(GLOB_RECURSE RenderPublic "render/public/*")
set(PublicSourceFileList 
	${CorePublic}
	${GeometryPublic}
	${RayTracingPublic}
	${RenderPublic}
	PARENT_SCOPE)

file(GLOB_RECURSE CorePrivate "core/private/*")
file(GLOB_RECURSE GeometryPrivate "geometry/private/*")
file(GLOB_RECURSE RayTracingPrivate "raytracing/private/*")
file(GLOB_RECURSE RenderPrivate "render/private/*")
set(PrivateSourceFileList 
	${CorePrivate}
	${GeometryPrivate}
	${RayTracingPrivate}
	${RenderPrivate}
	PARENT_SCOPE)

if (WIN32)
    
endif()