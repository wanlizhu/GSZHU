#pragma once

#if defined(_WIN32) || defined(_WIN64)
#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <tchar.h>
#include <shellapi.h>

#if defined(min)
#undef min
#endif

#if defined(max)
#undef max
#endif

#ifdef CreateWindow
#undef CreateWindow
#endif

#ifdef GetClassName
#undef GetClassName
#endif

#ifdef INFO
#undef INFO
#endif

#ifdef DEBUG
#undef DEBUG
#endif

#ifdef WARNING
#undef WARNING
#endif

#ifdef ERROR
#undef ERROR
#endif

#ifdef FATAL
#undef FATAL
#endif

#ifdef GetObject
#undef GetObject
#endif

#ifdef GetMessage
#undef GetMessage
#endif

#endif // #if defined(_WIN32) || defined(_WIN64)
