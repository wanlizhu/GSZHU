#pragma once

#include <string>
#include <unordered_map>
#include <optional>
#include <memory>

namespace Wanlix
{
    class DynamicLibrary final
    {
    public:
        using Ptr = std::shared_ptr<DynamicLibrary>;
        using Handle = void*;
        using Symbol = void*;
        using LoadedLibraries = std::unordered_map<std::wstring, Handle>;
        
        
        DynamicLibrary(const std::wstring& name);

        bool IsLoaded() const;
        bool Load();
        void Unload();
        const std::wstring& GetName() const;
        Symbol GetSymbol(const std::string& name) const;
       
    private:
        static LoadedLibraries smLoadedLibraries;
        const std::wstring mName;
    };
}