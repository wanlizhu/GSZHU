#pragma once

#include <string>
#include <unordered_map>
#include <optional>
#include <memory>

namespace UT
{
    class DynamicLibrary final
    {
    public:
        using Ptr = std::shared_ptr<DynamicLibrary>;
        using Handle = void*;
        using Symbol = void*;
        using LoadedLibraries = std::unordered_map<std::string, Handle>;
        
        
        DynamicLibrary(const std::string& name);

        bool IsLoaded() const;
        bool Load();
        void Unload();
        const std::string& GetName() const;
        Symbol GetSymbol(const std::string& name) const;
       
    private:
        static LoadedLibraries smLoadedLibraries;
        const std::string mName;
    };
}