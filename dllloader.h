#ifndef __DLLLOADER__H__
#define __DLLLOADER__H__

#ifndef _WIN32
#include "wintypes.h"
#else
#include <windows.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
HMODULE MyLoadLibrary(const char*dllname);
#ifdef _WIN32_WCE
HMODULE MyLoadKernelLibrary(const char*dllname);
#endif
FARPROC MyGetProcAddress(HMODULE hModule, const char*procname);
BOOL MyFreeLibrary(HMODULE hModule);

#define ERROR_INVALID_HANDLE             6L
#define ERROR_GEN_FAILURE                31L
#define ERROR_MOD_NOT_FOUND              126L
#define ERROR_PROC_NOT_FOUND             127L
void MySetLastError(unsigned err);
unsigned MyGetLastError();

#ifdef __cplusplus
}
#endif

#ifndef _WIN32
#define LoadLibrary MyLoadLibrary
#define LoadKernelLibrary MyLoadKernelLibrary
#define GetProcAddress MyGetProcAddress
#define FreeLibrary MyFreeLibrary
#define SetLastError MySetLastError
#define GetLastError MyGetLastError
#endif


#endif
