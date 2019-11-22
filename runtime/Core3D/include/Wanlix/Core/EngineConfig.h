#pragma once

#include <assert.h>

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif

    // identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
    #pragma warning(disable: 4251)
    // non – DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
    #pragma warning(disable: 4275)
    // no definition for inline function 'function'
    #pragma warning(disable: 4506)
#endif