#pragma once

#include "Core/Config.h"
#include "Core/Common.h"

namespace Wanlix
{
    using DllHandle = void*;
    
    EMessageBoxButton ShowMessageBox(const String& msg, EMessageBoxType type = EMessageBoxType::Ok);
    StringArray FindFiles(const String& filename, const String& dir, Bool recursive = true);
    Bool MakeDirectory(const String& path);
    Bool DeleteDirectory(const String& path);
    Bool OpenFileDialog(const StringArray& extFilters, String& filename);
    Bool SaveFileDialog(const StringArray& extFilters, String& filename);
    Bool FileExists(const String& filename, Bool normalFile = false);
    Bool DirectoryExists(const String& path);
    String GetExecutableName();
    String GetCWD();
    Bool IsDebuggerAttached();
    void DebugBreak();
    Optional<String> EnvironmentVariable(const String& name);
    size_t GetFileSize(const String& filename);
    String FileExtension(const String& str);
    String FileName(const String& str);
    String FileNameExt(const String& str);
    String FileParentPath(const String& str);
    String Canonicalize(const String& str);
    String JoinPath(const String& dir, const String& name);
    ByteArrayPtr ReadFile(const String& filename);
    DllHandle LoadDll(const String& filename, const StringArray& searchDirs = {});
    void* FindSymbolFromDll(DllHandle handle, const String& symbolName);
    void ReleaseDll(DllHandle handle);
}