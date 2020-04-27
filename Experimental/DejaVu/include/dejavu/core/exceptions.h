#pragma once

#include <exception>
#include <stdexcept>
#include <string>
#include <vector>
#include <type_traits>
#include "dejavu/core/preprocess.h"

namespace djv
{
    class DJV_API Exception : public std::exception
    {
    public:
        Exception() = default;
        Exception(const std::string& msg, int code)
            : mMessage(msg)
            , mCode(0)
        {}
        Exception(const std::string& msg, const std::string& args, int code)
            : mMessage(msg + ": " + args)
            , mCode(0)
        {}
        
        virtual ~Exception() = default;
        virtual const char* name() const noexcept { return "Exception"; };

        inline std::string message() const noexcept { return mMessage; };
        inline const char* what() const noexcept { return mMessage.c_str(); };
        inline int code() const noexcept { return mCode; }
        inline void rethrow() const { throw *this; };

    protected:
        void set_message(const std::string& msg) { mMessage = msg; }

    protected:
        int mCode = 0;
        std::string mMessage = "Exception";
    };

#define DECL_EXCEPTION_CLASS(API, ClassName, BaseClassName) \
    class API ClassName : public BaseClassName { \
    public: \
        explicit ClassName(const std::string& msg, int code) : BaseClassName(msg, code) {} \
        explicit ClassName(const std::string& msg, const std::string& args, int code) : BaseClassName(msg, args, code) {} \
        virtual const char* name() const noexcept { return #ClassName; } \
    };

    DECL_EXCEPTION_CLASS(DJV_API, LogicalException, Exception)
    DECL_EXCEPTION_CLASS(DJV_API, NullPointerException, LogicalException)
    DECL_EXCEPTION_CLASS(DJV_API, NullOptionalException, LogicalException)
    DECL_EXCEPTION_CLASS(DJV_API, AssertionFailedException, LogicalException)
    DECL_EXCEPTION_CLASS(DJV_API, NotImplementedException, LogicalException)
    DECL_EXCEPTION_CLASS(DJV_API, InvalidArgumentException, LogicalException)
    DECL_EXCEPTION_CLASS(DJV_API, InvalidAccessException, LogicalException)
    DECL_EXCEPTION_CLASS(DJV_API, SignalException, LogicalException)
    DECL_EXCEPTION_CLASS(DJV_API, UnhandledException, LogicalException)

    DECL_EXCEPTION_CLASS(DJV_API, RuntimeException, Exception)
    DECL_EXCEPTION_CLASS(DJV_API, NotFoundException, RuntimeException)
    DECL_EXCEPTION_CLASS(DJV_API, AlreadyExistsException, RuntimeException)
    DECL_EXCEPTION_CLASS(DJV_API, TimeoutException, RuntimeException)
    DECL_EXCEPTION_CLASS(DJV_API, SystemException, RuntimeException)
    DECL_EXCEPTION_CLASS(DJV_API, LoadLibraryException, RuntimeException)
    DECL_EXCEPTION_CLASS(DJV_API, AlreadyLoadedLibraryException, RuntimeException)
    DECL_EXCEPTION_CLASS(DJV_API, NoPermissionException, RuntimeException)
    DECL_EXCEPTION_CLASS(DJV_API, OutOfMemoryException, RuntimeException)

    DECL_EXCEPTION_CLASS(DJV_API, FileException, RuntimeException)
    DECL_EXCEPTION_CLASS(DJV_API, FileExistsException, FileException)
    DECL_EXCEPTION_CLASS(DJV_API, FileNotFoundException, FileException)
    DECL_EXCEPTION_CLASS(DJV_API, PathNotFoundException, FileException)
    DECL_EXCEPTION_CLASS(DJV_API, PathSyntaxException, FileException)
    DECL_EXCEPTION_CLASS(DJV_API, FileReadOnlyException, FileException)
    DECL_EXCEPTION_CLASS(DJV_API, FileAccessDeniedException, FileException)
    DECL_EXCEPTION_CLASS(DJV_API, CreateFileException, FileException)
    DECL_EXCEPTION_CLASS(DJV_API, OpenFileException, FileException)
    DECL_EXCEPTION_CLASS(DJV_API, WriteFileException, FileException)
    DECL_EXCEPTION_CLASS(DJV_API, ReadFileException, FileException)
    DECL_EXCEPTION_CLASS(DJV_API, DirectoryNotEmptyException, FileException)
    DECL_EXCEPTION_CLASS(DJV_API, UnknownURISchemeException, FileException)
    DECL_EXCEPTION_CLASS(DJV_API, URISyntaxException, FileException)

#undef DECL_EXCEPTION_CLASS
}