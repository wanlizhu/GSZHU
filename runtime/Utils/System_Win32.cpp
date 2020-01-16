#include "Utils/System.h"
#include "Utils/Strings.h"
#include <filesystem>
#include <Windows.h>
#include <commdlg.h>

namespace Wanlix
{
    String GetExtFilterString(const StringArray& filters, Bool isOpen)
    {
        String s;
        String d;
        bool appendForOpen = isOpen && filters.size() > 1;
        if (appendForOpen) {
            s.append(1, 0);
        }

        for (size_t i = 0; i < filters.size(); i++)
        {
            const auto& f = filters[i];
            if (appendForOpen)
            {
                bool last = i == (filters.size() - 1);
                std::string e = "*." + f;
                if (last == false) e += ';';
                d += e;
                s += e;
            }
            else
            {
                s += f + " files";
                s.append(1, 0);
                s += "*." + f + ';';
                s.append(1, 0);
            }
        }

        if (appendForOpen) {
            s = "Supported Formats (" + d + ')' + s;
        }
        s.append(appendForOpen ? 2 : 1, 0);

        return s;
    }

    Bool FileDialog(const StringArray& filters, String& filename, Bool isOpen)
    {
        OPENFILENAMEA ofn;
        CHAR chars[512] = "";
        ZeroMemory(&ofn, sizeof(ofn));

        String filtersString = GetExtFilterString(filters, isOpen);

        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = GetForegroundWindow();
        ofn.lpstrFilter = filtersString.c_str();
        ofn.lpstrFile = chars;
        ofn.nMaxFile = ArraySize(chars);
        ofn.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
        if (isOpen) {
            ofn.Flags |= OFN_FILEMUSTEXIST;
        }
        ofn.lpstrDefExt = "";

        BOOL b = isOpen ? GetOpenFileNameA(&ofn) : GetSaveFileNameA(&ofn);
        if (b)
        {
            filename = String(chars);
            if (FileExtension(filename).empty() && filters.empty() == false)
            {
                filename += '.' + filters[ofn.nFilterIndex];
            }
            return true;
        }
        return false;
    }

    EMessageBoxButton ShowMessageBox(const String& msg, EMessageBoxType type)
    {
        UINT Type = MB_OK;
        switch (type)
        {
        case EMessageBoxType::Ok:
            Type = MB_OK;
            break;
        case EMessageBoxType::OkCancel:
            Type = MB_OKCANCEL;
            break;
        case EMessageBoxType::RetryCancel:
            Type = MB_RETRYCANCEL;
            break;
        case EMessageBoxType::AbortRetryIgnore:
            Type = MB_ABORTRETRYIGNORE;
            break;
        default:
            assert(false);
            break;
        }

        int value = MessageBoxA(nullptr, msg.c_str(), "Wanlix", Type | MB_TOPMOST);
        switch (value)
        {
        case IDOK:
            return EMessageBoxButton::Ok;
        case IDCANCEL:
            return EMessageBoxButton::Cancel;
        case IDRETRY:
            return EMessageBoxButton::Retry;
        case IDABORT:
            return EMessageBoxButton::Abort;
        case IDIGNORE:
            return EMessageBoxButton::Ignore;
        default:
            assert(false);
            return EMessageBoxButton::Cancel;
        }
    }

    void FindFilesInternal(const String& name, const String& dir, Bool recursive, StringArray& result)
    {
        String szFilter = JoinPath(dir, name);
        WIN32_FIND_DATAA findData = {};

        HANDLE findHandle = FindFirstFileA(szFilter.c_str(), &findData);
        if (findHandle != INVALID_HANDLE_VALUE) 
        {
            do
            {
                if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    if (recursive)
                    {
                        if (!StringEqual2(findData.cFileName, ".", ".."))
                        {
                            String szNextDir = JoinPath(dir, findData.cFileName);
                            FindFilesInternal(szNextDir, name, recursive, result);
                        }
                    }
                }
                else
                {
                    result.emplace_back(JoinPath(dir, findData.cFileName));
                }
            } while (FindNextFileA(findHandle, &findData));
            FindClose(findHandle);
        }
    }

    StringArray FindFiles(const String& filename, const String& dir, Bool recursive)
    {
        String root = Canonicalize(dir);
        StringArray result;

        FindFilesInternal(filename, dir, recursive, result);
        return result;
    }

    Bool MakeDirectory(const String& path)
    {
        StringArray parts = SplitString(path, "/\\");
        String str;
        for (int i = 0; i < (int)parts.size(); i++)
        {
            if (parts[i].empty()) {
                continue;
            }
            str += parts[i];
            if (!FileExists(str))
            {
                if (!CreateDirectoryA(str.c_str(), NULL)) {
                    return false;
                }
            }
            str += "\\";
        }
        return true;
    }

    Bool DeleteDirectory(const String& path)
    {
        String szFind = JoinPath(path, "*");
        String szFind2;
        WIN32_FIND_DATAA findData = {};

        HANDLE findHandle = FindFirstFileA(szFind.c_str(), &findData);
        if (findHandle != INVALID_HANDLE_VALUE) 
        {
            do
            {
                szFind2 = JoinPath(path, findData.cFileName);
                if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    if (!StringEqual2(findData.cFileName, ".", ".."))
                    {
                        DeleteDirectory(szFind2);
                    }
                }
                else
                {
                    DeleteFileA(szFind2.c_str());
                }
            } while (FindNextFileA(findHandle, &findData));
            FindClose(findHandle);
        }
        return RemoveDirectoryA(path.c_str()) == TRUE;
    }

    Bool OpenFileDialog(const StringArray& extFilters, String& filename)
    {
        return FileDialog(extFilters, filename, true);
    }

    Bool SaveFileDialog(const StringArray& extFilters, String& filename)
    {
        return FileDialog(extFilters, filename, false);
    }

    Bool FileExists(const String& filename, Bool normalFile)
    {
        DWORD attr = GetFileAttributesA(filename.c_str());
        if (!normalFile) {
            return (attr != INVALID_FILE_ATTRIBUTES);
        }
        else {
            return (attr != INVALID_FILE_ATTRIBUTES) && ((attr & FILE_ATTRIBUTE_DIRECTORY) == 0);
        }
    }

    Bool DirectoryExists(const String& path)
    {
        DWORD attr = GetFileAttributesA(path.c_str());
        return ((attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY));
    }

    String GetExecutableName()
    {
        static String filename;
        if (filename.size() == 0)
        {
            CHAR exeName[MAX_PATH];
            GetModuleFileNameA(nullptr, exeName, ARRAYSIZE(exeName));
            const std::string tmp(exeName);

            auto last = tmp.find_last_of("/\\");
            filename = tmp.substr(last + 1, std::string::npos);
        }
        return filename;
    }

    String GetCWD()
    {
        CHAR curDir[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, curDir);
        return String(curDir);
    }

    Bool IsDebuggerAttached()
    {
        #ifdef _DEBUG
        return ::IsDebuggerPresent() == TRUE;
        #else
        return false;
        #endif
    }

    void DebugBreak()
    {
        __debugbreak();
    }

    Optional<String> EnvironmentVariable(const String& name)
    {
        static char buff[4096];
        int numChar = GetEnvironmentVariableA(name.c_str(), buff, ArraySize(buff)); //what is the best way to deal with wchar ?
        assert(numChar < ArraySize(buff));
        if (numChar == 0) {
            return std::nullopt;
        }
        return std::make_optional(std::string(buff));
    }

    size_t GetFileSize(const String& filename)
    {
        HANDLE file = CreateFileA(filename.c_str(), 
                                  GENERIC_READ,
                                  0,
                                  NULL,
                                  OPEN_EXISTING,
                                  0,
                                  NULL);
        if (file == INVALID_HANDLE_VALUE) {
            return 0;
        }

        LARGE_INTEGER size;
        if (GetFileSizeEx(file, &size) == FALSE) {
            CloseHandle(file);
            return 0;
        }

        CloseHandle(file);
        return size.QuadPart;
    }

    String FileExtension(const String& str)
    {
        std::filesystem::path path(str);
        if (path.has_extension()) {
            return path.extension().string();
        }
        return "";
    }

    String FileName(const String& str)
    {
        std::filesystem::path path(str);
        if (path.has_stem()) {
            return path.stem().string();
        }
        return "";
    }

    String FileNameExt(const String& str)
    {
        std::filesystem::path path(str);
        if (path.has_filename()) {
            return path.filename().string();
        }
        return "";
    }

    String FileParentPath(const String& str)
    {
        std::filesystem::path path(str);
        if (path.has_parent_path()) {
            return path.parent_path().string();
        }
        return "";
    }

    String Canonicalize(const String& str)
    {
        return ReplaceString(str, "/", "\\");
    }

    String JoinPath(const String& dir, const String& name)
    {
        bool endSlash = (dir.back() == '/' || dir.back() == '\\');
        return endSlash ? (dir + name) : (dir + "\\" + name);
    }

    ByteArray ReadFile(const String& filename)
    {
        size_t size = GetFileSize(filename);
        HANDLE file = CreateFileA(filename.c_str(),
                                  GENERIC_READ,
                                  0,
                                  NULL,
                                  OPEN_EXISTING,
                                  0,
                                  NULL);
        if (file == INVALID_HANDLE_VALUE) {
            return 0;
        }

        ByteArray buffer = std::make_shared<ByteArray>(size);
        assert(buffer->size() == size);
        DWORD readCount = 0;

        if (FALSE == ::ReadFile(file, buffer->data(), (DWORD)size, &readCount, NULL)) {
            buffer->clear();
            CloseHandle(file);
            return nullptr;
        }

        CloseHandle(file);
        return buffer;
    }

    DllHandle LoadDll(const String& filename, const StringArray& searchDirs)
    {
        String path = filename;
        if (!FileExists(path, true))
        {
            for (auto& dirName : searchDirs)
            {
                for (auto& filePath : FindFiles(filename, dirName, true))
                {
                    path = filePath;
                    break;
                }
                if (FileExists(path, true)) {
                    break;
                }
            }
        }
        return LoadLibraryA(path.c_str());
    }

    void* FindSymbolFromDll(DllHandle handle, const String& symbolName)
    {
        return GetProcAddress((HMODULE)handle, symbolName.c_str());
    }

    void ReleaseDll(DllHandle handle)
    {
        FreeLibrary((HMODULE)handle);
    }

}