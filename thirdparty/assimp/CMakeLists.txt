CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(Assimp)

SET(BUILD_SHARED_LIBS OFF)
SET(BUILD_FRAMEWORK OFF)
SET(ASSIMP_DOUBLE_PRECISION OFF)
SET(ASSIMP_OPT_BUILD_PACKAGES OFF)
SET(ASSIMP_ANDROID_JNIIOSYSTEM OFF)
SET(ASSIMP_NO_EXPORT ON)
SET(ASSIMP_BUILD_ZLIB OFF)
SET(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
SET(ASSIMP_BUILD_SAMPLES OFF)
SET(ASSIMP_BUILD_TESTS OFF)
SET(ASSIMP_COVERALLS OFF)
SET(ASSIMP_WERROR OFF)
SET(ASSIMP_ASAN OFF)
SET(ASSIMP_UBSAN OFF)
SET(SYSTEM_IRRXML OFF)
SET(BUILD_DOCS OFF)

if (WIN32)
  ADD_DEFINITIONS(-DWIN32_LEAN_AND_MEAN)
endif()

IF(MSVC)
  SET(CMAKE_PREFIX_PATH "D:\\libs\\devil")
  SET(ASSIMP_INSTALL_PDB ON)
ENDIF(MSVC)

IF (BUILD_FRAMEWORK)
  SET(BUILD_SHARED_LIBS ON)
  MESSAGE(STATUS "Framework bundle building enabled")
ENDIF(BUILD_FRAMEWORK)

IF(NOT BUILD_SHARED_LIBS)
  MESSAGE(STATUS "Shared libraries disabled")
  SET(LINK_SEARCH_START_STATIC TRUE)
ELSE()
  MESSAGE(STATUS "Shared libraries enabled")
ENDIF(NOT BUILD_SHARED_LIBS)

# Define here the needed parameters
SET(ASSIMP_VERSION_MAJOR 4)
SET(ASSIMP_VERSION_MINOR 1)
SET(ASSIMP_VERSION_PATCH 0)
SET(ASSIMP_VERSION ${ASSIMP_VERSION_MAJOR}.${ASSIMP_VERSION_MINOR}.${ASSIMP_VERSION_PATCH})
SET(ASSIMP_SOVERSION 4)
SET(PROJECT_VERSION "${ASSIMP_VERSION}")

SET(ASSIMP_PACKAGE_VERSION "0")

# Needed for openddl_parser config, no use of c++11 at this moment
ADD_DEFINITIONS(-DOPENDDL_NO_USE_CPP11)
set_property(GLOBAL PROPERTY CXX_STANDARD 11)

# Get the current working branch
EXECUTE_PROCESS(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_QUIET
)

# Get the latest abbreviated commit hash of the working branch
EXECUTE_PROCESS(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_QUIET
)

IF(NOT GIT_COMMIT_HASH)
    SET(GIT_COMMIT_HASH 0)
ENDIF(NOT GIT_COMMIT_HASH)

IF(ASSIMP_DOUBLE_PRECISION)
    ADD_DEFINITIONS(-DASSIMP_DOUBLE_PRECISION)
ENDIF(ASSIMP_DOUBLE_PRECISION)

CONFIGURE_FILE(
  ${CMAKE_CURRENT_LIST_DIR}/revision.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/revision.h
)

CONFIGURE_FILE(
  ${CMAKE_CURRENT_LIST_DIR}/include/assimp/config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/include/assimp/config.h
)

INCLUDE_DIRECTORIES(
    ./
	include
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules" )
SET(LIBASSIMP_COMPONENT     "libassimp${ASSIMP_VERSION_MAJOR}.${ASSIMP_VERSION_MINOR}.${ASSIMP_VERSION_PATCH}" )
SET(LIBASSIMP-DEV_COMPONENT "libassimp${ASSIMP_VERSION_MAJOR}.${ASSIMP_VERSION_MINOR}.${ASSIMP_VERSION_PATCH}-dev" )
SET(CPACK_COMPONENTS_ALL assimp-bin ${LIBASSIMP_COMPONENT} ${LIBASSIMP-DEV_COMPONENT} assimp-dev)
SET(ASSIMP_LIBRARY_SUFFIX "")

IF( UNIX )
  # Ensure that we do not run into issues like http://www.tcm.phy.cam.ac.uk/sw/inodes64.html on 32 bit linux
  IF( ${OPERATING_SYSTEM} MATCHES "Android")
  ELSE()
    IF ( CMAKE_SIZEOF_VOID_P EQUAL 4) # only necessary for 32-bit linux
      #ADD_DEFINITIONS(-D_FILE_OFFSET_BITS=64 )
    ENDIF()
  ENDIF()

  # Use GNUInstallDirs for Unix predefined directories
  INCLUDE(GNUInstallDirs)
ENDIF( UNIX )


# Grouped compiler settings
IF ((CMAKE_C_COMPILER_ID MATCHES "GNU") AND NOT CMAKE_COMPILER_IS_MINGW)
  # hide all not-exported symbols
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fvisibility=hidden -fPIC -Wall -std=c++0x")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
  SET(LIBSTDC++_LIBRARIES -lstdc++)
ELSEIF(MSVC)
  # enable multi-core compilation with MSVC
  add_compile_options(/MP)

  # disable "elements of array '' will be default initialized" warning on MSVC2013
  IF(MSVC12)
    add_compile_options(/wd4351)
  ENDIF()
ELSEIF ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" )
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fvisibility=hidden -fPIC -Wall -Wno-long-long -std=c++11" )
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
ELSEIF( CMAKE_COMPILER_IS_MINGW )
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wall -Wno-long-long -std=c++11" )
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
  ADD_DEFINITIONS( -U__STRICT_ANSI__ )
ENDIF()

if (ASSIMP_COVERALLS)
    MESSAGE(STATUS "Coveralls enabled")
    INCLUDE(Coveralls)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
endif()

if (ASSIMP_WERROR)
  MESSAGE(STATUS "Treating warnings as errors")
  IF (MSVC)
    add_compile_options(/WX)
  ELSE()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
  ENDIF()
endif()

if (ASSIMP_ASAN)
    MESSAGE(STATUS "AddressSanitizer enabled")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
endif()

if (ASSIMP_UBSAN)
    MESSAGE(STATUS "Undefined Behavior sanitizer enabled")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -fno-sanitize-recover=all")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined -fno-sanitize-recover=all")
endif()

INCLUDE(FindPkgMacros)
INCLUDE(PrecompiledHeader)

# If this is an in-source build (CMAKE_SOURCE_DIR == CMAKE_BINARY_DIR),
# write the library/executable files to the respective directories in the
# source tree. During an out-of-source build, however, do not litter this
# directory, since that is probably what the user wanted to avoid.
IF (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR )
  SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_HOME_DIRECTORY}/lib" )
  SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_HOME_DIRECTORY}/lib" )
  SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_HOME_DIRECTORY}/bin" )
ENDIF(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR )

# Cache these to allow the user to override them manually.
SET(ASSIMP_LIB_INSTALL_DIR "lib")
SET(ASSIMP_INCLUDE_INSTALL_DIR "include")
SET(ASSIMP_BIN_INSTALL_DIR "bin")

IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
  SET(CMAKE_DEBUG_POSTFIX "d")
ELSE()
  SET(CMAKE_DEBUG_POSTFIX "")
ENDIF()

# Only generate this target if no higher-level project already has
# IF (NOT TARGET uninstall)
#   # add make uninstall capability
#   CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
#   add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
# ENDIF()

# cmake configuration files
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/assimp-config.cmake.in"         "${CMAKE_CURRENT_BINARY_DIR}/assimp-config.cmake" @ONLY IMMEDIATE)
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/assimp-config-version.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/assimp-config-version.cmake" @ONLY IMMEDIATE)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/assimp-config.cmake"             "${CMAKE_CURRENT_BINARY_DIR}/assimp-config-version.cmake" DESTINATION "${ASSIMP_LIB_INSTALL_DIR}/cmake/assimp-${ASSIMP_VERSION_MAJOR}.${ASSIMP_VERSION_MINOR}" COMPONENT ${LIBASSIMP-DEV_COMPONENT})

FIND_PACKAGE( DirectX )

IF( BUILD_DOCS )
    add_subdirectory(doc)
ENDIF( BUILD_DOCS )

# Look for system installed irrXML
IF ( SYSTEM_IRRXML )
    find_package( IrrXML REQUIRED )
ENDIF( SYSTEM_IRRXML )

# Search for external dependencies, and build them from source if not found
# Search for zlib
IF ( NOT ASSIMP_BUILD_ZLIB )
    find_package(ZLIB)
ENDIF( NOT ASSIMP_BUILD_ZLIB )

IF( NOT ZLIB_FOUND )
  message(STATUS "compiling zlib from souces")
  INCLUDE(CheckIncludeFile)
  INCLUDE(CheckTypeSize)
  INCLUDE(CheckFunctionExists)
  # compile from sources
  add_subdirectory(contrib/zlib)
  SET(ZLIB_FOUND 1)
  SET(ZLIB_LIBRARIES zlibstatic)
  SET(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/contrib/zlib ${CMAKE_CURRENT_BINARY_DIR}/contrib/zlib)
else(NOT ZLIB_FOUND)
  ADD_DEFINITIONS(-DASSIMP_BUILD_NO_OWN_ZLIB)
  SET(ZLIB_LIBRARIES_LINKED -lz)
ENDIF(NOT ZLIB_FOUND)
INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})

# Search for unzip
use_pkgconfig(UNZIP minizip)

IF ( ASSIMP_NO_EXPORT )
  ADD_DEFINITIONS( -DASSIMP_BUILD_NO_EXPORT)
  MESSAGE( STATUS "Build an import-only version of Assimp." )
ENDIF( ASSIMP_NO_EXPORT )

SET ( ASSIMP_BUILD_ARCHITECTURE "")
IF    ( ASSIMP_BUILD_ARCHITECTURE STREQUAL "")
ELSE  ( ASSIMP_BUILD_ARCHITECTURE STREQUAL "")
  ADD_DEFINITIONS ( -D'ASSIMP_BUILD_ARCHITECTURE="${ASSIMP_BUILD_ARCHITECTURE}"' )
ENDIF ( ASSIMP_BUILD_ARCHITECTURE STREQUAL "")

# ${CMAKE_GENERATOR}
SET ( ASSIMP_BUILD_COMPILER "")
IF    ( ASSIMP_BUILD_COMPILER STREQUAL "" )
ELSE  ( ASSIMP_BUILD_COMPILER STREQUAL "" )
  ADD_DEFINITIONS ( -D'ASSIMP_BUILD_COMPILER="${ASSIMP_BUILD_COMPILER}"' )
ENDIF ( ASSIMP_BUILD_COMPILER STREQUAL "")

MARK_AS_ADVANCED ( ASSIMP_BUILD_ARCHITECTURE ASSIMP_BUILD_COMPILER )

SET ( ASSIMP_BUILD_NONFREE_C4D_IMPORTER OFF )

IF (ASSIMP_BUILD_NONFREE_C4D_IMPORTER)
  IF ( MSVC )
    SET(C4D_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/contrib/Melange/includes")

    # pick the correct prebuilt library
    IF(MSVC14)
      SET(C4D_LIB_POSTFIX "_2015")
    ELSEIF(MSVC12)
      SET(C4D_LIB_POSTFIX "_2013")
    ELSEIF(MSVC11)
      SET(C4D_LIB_POSTFIX "_2012")
    ELSEIF(MSVC10)
      SET(C4D_LIB_POSTFIX "_2010")
    ELSE()
      MESSAGE( FATAL_ERROR
        "C4D is currently only supported with MSVC 10, 11, 12, 14"
      )
    ENDIF()

    SET(C4D_LIB_BASE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/contrib/Melange/libraries/win")

    SET(C4D_DEBUG_LIBRARIES
      "${C4D_LIB_BASE_PATH}/melangelib${C4D_LIB_POSTFIX}/melangelib_debug.lib"
      "${C4D_LIB_BASE_PATH}/jpeglib${C4D_LIB_POSTFIX}/jpeglib_debug.lib"
    )
    SET(C4D_RELEASE_LIBRARIES
      "${C4D_LIB_BASE_PATH}/melangelib${C4D_LIB_POSTFIX}/melangelib_release.lib"
      "${C4D_LIB_BASE_PATH}/jpeglib${C4D_LIB_POSTFIX}/jpeglib_release.lib"
    )

    # winsock and winmm are necessary dependencies of melange (this is undocumented, but true.)
    SET(C4D_EXTRA_LIBRARIES WSock32.lib Winmm.lib)
  ELSE ()
    MESSAGE( FATAL_ERROR
      "C4D is currently only available on Windows with melange SDK installed in contrib/Melange"
    )
  ENDIF ( MSVC )
ELSE (ASSIMP_BUILD_NONFREE_C4D_IMPORTER)
  ADD_DEFINITIONS( -DASSIMP_BUILD_NO_C4D_IMPORTER )
ENDIF (ASSIMP_BUILD_NONFREE_C4D_IMPORTER)

ADD_SUBDIRECTORY(contrib)

ADD_SUBDIRECTORY( code/ )
IF ( ASSIMP_BUILD_ASSIMP_TOOLS )
  IF ( WIN32 AND DirectX_D3DX9_LIBRARY )
    SET ( ASSIMP_BUILD_ASSIMP_VIEW ${DirectX_FOUND} )
    IF ( ASSIMP_BUILD_ASSIMP_VIEW )
      ADD_SUBDIRECTORY( tools/assimp_view/ )
    ENDIF ( ASSIMP_BUILD_ASSIMP_VIEW )
  ENDIF ( WIN32 AND DirectX_D3DX9_LIBRARY )

  ADD_SUBDIRECTORY( tools/assimp_cmd/ )

  # Check dependencies for assimp_qt_viewer.
  # Why here? Maybe user do not want Qt viewer and have no Qt.
  # Why assimp_qt_viewer/CMakeLists.txt still contain similar check?
  # Because viewer can be build independently of Assimp.
  FIND_PACKAGE(Qt5Widgets QUIET)
  FIND_PACKAGE(DevIL QUIET)
  FIND_PACKAGE(OpenGL QUIET)
  IF ( Qt5Widgets_FOUND AND IL_FOUND AND OPENGL_FOUND)
    ADD_SUBDIRECTORY( tools/assimp_qt_viewer/ )
  ELSE()
    SET ( ASSIMP_QT_VIEWER_DEPENDENCIES "")
    IF (NOT Qt5_FOUND)
      SET ( ASSIMP_QT_VIEWER_DEPENDENCIES "${ASSIMP_QT_VIEWER_DEPENDENCIES} Qt5")
    ENDIF (NOT Qt5_FOUND)

    IF (NOT IL_FOUND)
      SET ( ASSIMP_QT_VIEWER_DEPENDENCIES "${ASSIMP_QT_VIEWER_DEPENDENCIES} DevIL")
    ENDIF (NOT IL_FOUND)

    IF (NOT OPENGL_FOUND)
      SET ( ASSIMP_QT_VIEWER_DEPENDENCIES "${ASSIMP_QT_VIEWER_DEPENDENCIES} OpengGL")
    ENDIF (NOT OPENGL_FOUND)

    MESSAGE (WARNING "Build of assimp_qt_viewer is disabled. Unsatisfied dendencies: ${ASSIMP_QT_VIEWER_DEPENDENCIES}")
  ENDIF ( Qt5Widgets_FOUND AND IL_FOUND AND OPENGL_FOUND)
ENDIF ( ASSIMP_BUILD_ASSIMP_TOOLS )

IF ( ASSIMP_BUILD_SAMPLES)
  IF ( WIN32 )
    ADD_SUBDIRECTORY( samples/SimpleTexturedOpenGL/ )
  ENDIF ( WIN32 )
  ADD_SUBDIRECTORY( samples/SimpleOpenGL/ )
ENDIF ( ASSIMP_BUILD_SAMPLES )

# IF ( ASSIMP_BUILD_TESTS )
#   ADD_SUBDIRECTORY( test/ )
# ENDIF ( ASSIMP_BUILD_TESTS )

# Generate a pkg-config .pc for the Assimp library.
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/assimp.pc.in" "${PROJECT_BINARY_DIR}/assimp.pc" @ONLY )
INSTALL( FILES "${PROJECT_BINARY_DIR}/assimp.pc" DESTINATION ${ASSIMP_LIB_INSTALL_DIR}/pkgconfig/ COMPONENT ${LIBASSIMP-DEV_COMPONENT})

IF(CMAKE_CPACK_COMMAND AND UNIX AND ASSIMP_OPT_BUILD_PACKAGES)
  # Packing information
  SET(CPACK_PACKAGE_NAME                    "assimp{ASSIMP_VERSION_MAJOR}")
  SET(CPACK_PACKAGE_CONTACT "" CACHE STRING "Package maintainer and PGP signer.")
  SET(CPACK_PACKAGE_VENDOR                  "https://github.com/assimp")
  SET(CPACK_PACKAGE_DISPLAY_NAME            "Assimp ${ASSIMP_VERSION}")
  SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY     " - Open Asset Import Library ${ASSIMP_VERSION}")
  SET(CPACK_PACKAGE_VERSION                 "${ASSIMP_VERSION}.${ASSIMP_PACKAGE_VERSION}" )
  SET(CPACK_PACKAGE_VERSION_MAJOR           "${ASSIMP_VERSION_MAJOR}")
  SET(CPACK_PACKAGE_VERSION_MINOR           "${ASSIMP_VERSION_MINOR}")
  SET(CPACK_PACKAGE_VERSION_PATCH           "${ASSIMP_VERSION_PATCH}")
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY       "assimp${ASSIMP_VERSION_MAJOR}.${ASSIMP_VERSION_MINOR}")
  SET(CPACK_RESOURCE_FILE_LICENSE           "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

  string(TOUPPER ${LIBASSIMP_COMPONENT}     "LIBASSIMP_COMPONENT_UPPER")
  string(TOUPPER ${LIBASSIMP-DEV_COMPONENT} "LIBASSIMP-DEV_COMPONENT_UPPER")

  SET(CPACK_COMPONENT_ASSIMP-BIN_DISPLAY_NAME                       "tools")
  SET(CPACK_COMPONENT_ASSIMP-BIN_DEPENDS                            "${LIBASSIMP_COMPONENT}" )
  SET(CPACK_COMPONENT_${LIBASSIMP_COMPONENT_UPPER}_DISPLAY_NAME     "libraries")
  SET(CPACK_COMPONENT_${LIBASSIMP-DEV_COMPONENT_UPPER}_DISPLAY_NAME "common headers and installs")
  SET(CPACK_COMPONENT_${LIBASSIMP-DEV_COMPONENT_UPPER}_DEPENDS $    "{LIBASSIMP_COMPONENT}" )
  SET(CPACK_COMPONENT_ASSIMP-DEV_DISPLAY_NAME                       "${CPACK_COMPONENT_${LIBASSIMP-DEV_COMPONENT}_DISPLAY_NAME}" )
  SET(CPACK_COMPONENT_ASSIMP-DEV_DEPENDS                            "${LIBASSIMP-DEV_COMPONENT}" )
  SET(CPACK_DEBIAN_BUILD_DEPENDS debhelper cmake zlib1g-dev pkg-config)

  # debian
  SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
  SET(CPACK_DEBIAN_CMAKE_OPTIONS    "-DBUILD_ASSIMP_SAMPLES:BOOL=${ASSIMP_BUILD_SAMPLES}")
  SET(CPACK_DEBIAN_PACKAGE_SECTION  "libs" )
  SET(CPACK_DEBIAN_PACKAGE_DEPENDS  "${CPACK_COMPONENTS_ALL}")
  SET(CPACK_DEBIAN_PACKAGE_SUGGESTS)
  SET(CPACK_DEBIAN_PACKAGE_NAME     "assimp")
  SET(CPACK_DEBIAN_PACKAGE_REMOVE_SOURCE_FILES contrib/cppunit-1.12.1 contrib/cppunit_note.txt contrib/zlib workspaces test doc obj samples packaging)
  SET(CPACK_DEBIAN_PACKAGE_SOURCE_COPY svn export --force)
  SET(CPACK_DEBIAN_CHANGELOG)
  execute_process(COMMAND lsb_release -is
    OUTPUT_VARIABLE _lsb_distribution OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE _lsb_release_failed)
  SET(CPACK_DEBIAN_DISTRIBUTION_NAME ${_lsb_distribution} )
  STRING(TOLOWER ${CPACK_DEBIAN_DISTRIBUTION_NAME} CPACK_DEBIAN_DISTRIBUTION_NAME)
  IF( ${CPACK_DEBIAN_DISTRIBUTION_NAME} STREQUAL "ubuntu" )
    SET(CPACK_DEBIAN_DISTRIBUTION_RELEASES lucid maverick natty oneiric precise )
  ENDIF()
  SET(DPUT_HOST "" CACHE STRING "PPA repository to upload the debian sources")
  include(CPack)
  include(DebSourcePPA)
ENDIF()

if(WIN32)
  if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(BIN_DIR "${PROJECT_SOURCE_DIR}/bin64/")
    SET(LIB_DIR "${PROJECT_SOURCE_DIR}/lib64/")
  elseif()
    SET(BIN_DIR "${PROJECT_SOURCE_DIR}/bin32/")
    SET(LIB_DIR "${PROJECT_SOURCE_DIR}/lib32/")
  ENDIF()

  if(MSVC12)
    SET(ASSIMP_MSVC_VERSION "vc120")
  elseif(MSVC14)
    SET(ASSIMP_MSVC_VERSION "vc140")
  ENDIF(MSVC12)
ENDIF (WIN32)
