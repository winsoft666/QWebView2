#if defined(_MSVC_) && _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER > 1000

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
#if defined(QWEBVIEW2_LIB)
#define QWEBVIEW2_EXPORT Q_DECL_EXPORT
#if (defined Q_OS_WIN32 || defined Q_OS_WIN64)
#pragma comment(lib, "Urlmon.lib")
#pragma comment(lib, "D2d1.lib")
#pragma comment(lib, "Shlwapi.lib")
#endif
#else
#define QWEBVIEW2_EXPORT Q_DECL_IMPORT
#endif
#else
#define QWEBVIEW2_EXPORT
#if (defined Q_OS_WIN32 || defined Q_OS_WIN64)
#pragma comment(lib, "Urlmon.lib")
#pragma comment(lib, "D2d1.lib")
#pragma comment(lib, "Shlwapi.lib")
#endif
#endif
