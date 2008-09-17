#ifndef __DLLLOADER__H__
#define __DLLLOADER__H__

#include "wintypes.h"

HMODULE LoadLibrary(const char*dllname);
void* GetProcAddress(HMODULE hModule, const char*procname);
BOOL FreeLibrary(HMODULE hModule);

#define ERROR_INVALID_HANDLE             6L
#define ERROR_GEN_FAILURE                31L
#define ERROR_MOD_NOT_FOUND              126L
#define ERROR_PROC_NOT_FOUND             127L
void SetLastError(unsigned err);
unsigned GetLastError();

#endif
