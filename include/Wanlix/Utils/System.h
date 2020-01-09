#pragma once

#include "Wanlix/Common.h"
#include "Wanlix/Utils/Flags.h"
#include <Windows.h>
namespace Wanlix
{
    using DllHandle = void*;
    
    EMessageBoxButton ShowMessageBox(String const& msg, EMessageBoxType type = EMessageBoxType::Ok);
    StringArray FindFiles(String const& filename, String const& dir, Bool recursive = true);
    Bool MakeDirectory(String const& path);
    Bool DeleteDirectory(String const& path);
    Bool OpenFileDialog(StringArray const& extFilters, String& filename);
    Bool SaveFileDialog(StringArray const& extFilters, String& filename);
    Bool FileExists(String const& filename, Bool normalFile = false);
    Bool DirectoryExists(String const& path);
    String GetExecutableName();
    String GetCWD();
    Bool IsDebuggerAttached();
    void DebugBreak();
    Optional<String> EnvironmentVariable(String const& name);
    size_t GetFileSize(String const& filename);
    String FileExtension(String const& str);
    String FileName(String const& str);
    String FileNameExt(String const& str);
    String FileParentPath(String const& str);
    String Canonicalize(String const& str);
    String JoinPath(String const& dir, String const& name);
    ByteArrayPtr ReadFile(String const& filename);
    DllHandle LoadDll(String const& filename, StringArray const& searchDirs = {});
    void* FindSymbolFromDll(DllHandle handle, String const& symbolName);
    void ReleaseDll(DllHandle handle);
}