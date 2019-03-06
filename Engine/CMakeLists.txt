CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(NTGS)

SET(TARGET NTGS)
SET(DEFINITIONS "")
SET(PUBLIC_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/API)
SET(SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Source)
SET(LINK_LIBS assimp)

FIND_PACKAGE(opengl REQUIRED)

# ===============================================
# Collect source and header files
# ===============================================
FILE(GLOB_RECURSE PUBLIC_HEADERS ${PUBLIC_INCLUDE_PATH}/NTGS/*.h)
FILE(GLOB_RECURSE PRIVATE_HEADERS ${SOURCE_PATH}/NTGS/*.h)
FILE(GLOB_RECURSE NTGS_SOURCES ${SOURCE_PATH}/NTGS/*.cpp)
FILE(GLOB_RECURSE NTGS_EXTERNAL_SOURCES ${SOURCE_PATH}/ThirdParty/stb/*.cpp)
SOURCE_GROUP(TREE ${PUBLIC_INCLUDE_PATH}/NTGS PREFIX "Header Files/NTGS" FILES ${PUBLIC_HEADERS})
SOURCE_GROUP(TREE ${SOURCE_PATH} PREFIX "Header Files/Private" FILES ${PRIVATE_HEADERS})


# ===============================================
# Specify compile definitions
# ===============================================
IF(WIN32)
    
ENDIF()

ADD_SUBDIRECTORY(Source/ThirdParty/assimp)

# ===============================================
# Target definition
# ===============================================
ADD_LIBRARY(${TARGET} STATIC ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${NTGS_SOURCES} ${NTGS_EXTERNAL_SOURCES})
TARGET_COMPILE_DEFINITIONS(${TARGET} PUBLIC ${DEFINITIONS})
TARGET_INCLUDE_DIRECTORIES(${TARGET} PUBLIC ${PUBLIC_INCLUDE_PATH} PRIVATE ${SOURCE_PATH})
TARGET_LINK_LIBRARIES(${TARGET} PRIVATE ${LINK_LIBS})

# ===============================================
# Installation
# ===============================================
INSTALL(TARGETS ${TARGET} ARCHIVE DESTINATION lib)
INSTALL(DIRECTORY ${PUBLIC_INCLUDE_PATH}/NTGS DESTINATION include)

ADD_SUBDIRECTORY(Tests)
