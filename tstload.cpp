#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>

#ifdef _USE_WINDOWS
#include <windows.h>
#else
#include "dllloader.h"
#endif

bool loaddll(const char *dllname)
{
    HMODULE hDll= LoadLibrary(dllname);
    if (hDll==NULL) {
        printf("ERROR - loadlib: %08x\n", GetLastError());
        return false;
    }
    if (!FreeLibrary(hDll)) {
        printf("ERROR - freelib: %08x\n", GetLastError());
        return false;
    }
    return true;
}
int main(int argc, char **argv)
{
    for (int i=1 ; i<argc ; i++)
        loaddll(argv[i]);
    return 0;
}
