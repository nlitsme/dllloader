#ifndef _WINTYPES_H_
#define _WINTYPES_H_

// some windows types:
typedef int BOOL;
typedef void* HMODULE;
typedef void* HANDLE;
typedef uint32_t DWORD;
typedef uint32_t* PDWORD;
typedef uint8_t BYTE;
typedef uint8_t* LPBYTE;
typedef void* LPVOID;
typedef void* PVOID;
typedef void VOID;
typedef const void* LPCVOID;
typedef uint16_t WORD;

#define INVALID_HANDLE_VALUE ((HANDLE)0xFFFFFFFF)

#endif
