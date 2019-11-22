#include "Wanlix/DynamicLibrary.h"
#define WIN32_LEAN_AND_MEAN
#if !defined(NOMINMAX) && defined(_MSC_VER)
    #define NOMINMAX // required to stop windows.h messing up std::min
#endif
#include <Windows.h>

#ifdef _WIN32
    #define LIB_EXT ".dll"
    #define LOAD_LIB(name) LoadLibraryEx(name, NULL, LOAD_WITH_ALTERED_SEARCH_PATH)
    #define GET_SYMBOL(lib, sym) GetProcAddress((HMODULE)lib, sym)
    #define UNLOAD_LIB(lib) FreeLibrary((HMODULE)lib)
#else
    #ifdef __APPLE_CC__
        #include <dlfcn.h>
        #define LIB_EXT ".dylib"
    #else 
        #define LIB_EXT ".so"
    #endif
    #define LOAD_LIB(name) dlopen(name, RTLD_LAZY | RTLD_GLOBAL)
    #define GET_SYMBOL(lib, sym) dlsym(lib, sym)
    #define UNLOAD_LIB(lib) dlclose(lib)
#endif

namespace UT
{
    DynamicLibrary::DynamicLibrary(const std::string& name)
        : mName(name)
    {}

    bool DynamicLibrary::IsLoaded() const
    {
        return smLoadedLibraries.find(mName) != smLoadedLibraries.end();
    }

    bool DynamicLibrary::Load()
    {
        if (IsLoaded()) {
            return true;
        }
        
        Handle handle = (Handle)LOAD_LIB(mName.c_str());
        if (!handle) {
            return false;
        }

        smLoadedLibraries[mName] = handle;
        return true;
    }

    void DynamicLibrary::Unload()
    {
        if (!IsLoaded()) {
            return;
        }

        Handle handle = smLoadedLibraries[mName];
        if (handle) {
            UNLOAD_LIB(handle);
        }
    }

    const std::string& DynamicLibrary::GetName() const
    {
        return mName;
    }

    DynamicLibrary::Symbol DynamicLibrary::GetSymbol(const std::string& name) const
    {
        if (!IsLoaded()) {
            return nullptr;
        }
        Handle handle = smLoadedLibraries.at(mName);
        return GET_SYMBOL(handle, name.c_str());
    }
}