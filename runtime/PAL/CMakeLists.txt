add_library(PAL STATIC "${RSD}/dummy.cpp")

function(RemoveFilesFromList _files_)
    set(options "")
    set(oneValueArgs "")
    set(multiValueArgs IN STARTS_WITH ENDS_WITH)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(_result_)

    if (ARGS_IN)
        foreach (_file_ ${${_files_}})
            foreach (_dir_ ${ARGS_IN})
                file(RELATIVE_PATH _relativePath_ ${_dir_} ${_file_})
                string(FIND ${_relativePath_} ".." _index_)
                if (${_index_} EQUAL 0)
                    list(APPEND _result_ ${_file_})
                endif()
            endforeach()
        endforeach()
    endif()

    if (ARGS_STARTS_WITH)
        message(FATAL_ERROR "TODO")
    endif()

    if (ARGS_ENDS_WITH)
        message(FATAL_ERROR "TODO")
    endif()

    set(${_files_} ${_result_} PARENT_SCOPE)
endfunction()

SetTargetOutputDirectories(PAL)
GetCurrentDirectorySourceFiles(_interfaces_ _headers_ _sources_)
set(all ${_interfaces_} ${_headers_} ${_sources_})

set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
RemoveFilesFromList(all IN "${CSD}/source/Win32")
message(${all})

target_sources(PAL PRIVATE ${_interfaces_} ${_headers_} ${_sources_})


target_include_directories(PAL PUBLIC "${CSD}/include")
target_include_directories(PAL PRIVATE "${RSD}/include")
target_include_directories(PAL PRIVATE "${CSD}/source")
target_compile_definitions(PAL PRIVATE "_UNICODE" "UNICODE")

get_target_property(_outdir_ PAL LIBRARY_OUTPUT_DIRECTORY)
message(STATUS "Target: PAL -> ${_outdir_}" )