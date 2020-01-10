function (SetOutputDirs
    _targetName_
)
    get_target_property(_outdir_ ${_targetName_} RUNTIME_OUTPUT_DIRECTORY)
    get_target_property(_libdir_ ${_targetName_} LIBRARY_OUTPUT_DIRECTORY)
    set_target_properties(${_targetName_} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${_outdir_}")
    set_target_properties(${_targetName_} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${_outdir_}")
    set_target_properties(${_targetName_} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINISIZEREL    "${_outdir_}")
    set_target_properties(${_targetName_} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${_outdir_}")
    set_target_properties(${_targetName_} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${_libdir_}")
    set_target_properties(${_targetName_} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${_libdir_}")
    set_target_properties(${_targetName_} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_MINISIZEREL    "${_libdir_}")
    set_target_properties(${_targetName_} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${_libdir_}")
endfunction()