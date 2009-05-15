#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>

// gobjdump -D -m i386 -b binary --start-address=$[0x1000] --stop-address=$[0x3604] CECompressv3.dll
// gobjdump -D -m i386 -b binary --start-address=$[0x1000] --stop-address=$[0x6935] CECompressv4.dll
// gobjdump -D -m i386 -b binary --start-address=$[0x1000] --stop-address=$[0x7030] cecompr_nt.dll
//
#ifdef _USE_WINDOWS
#include <windows.h>
#else
#include "dllloader.h"
#endif

typedef DWORD (*CECOMPRESS)(const LPBYTE  lpbSrc, DWORD cbSrc, LPBYTE lpbDest, DWORD cbDest, WORD wStep, DWORD dwPagesize);
typedef DWORD (*CEDECOMPRESS)(const LPBYTE  lpbSrc, DWORD cbSrc, LPBYTE  lpbDest, DWORD cbDest, DWORD dwSkip, WORD wStep, DWORD dwPagesize);

typedef LPVOID (*FNCompressAlloc)(DWORD AllocSize);
typedef VOID (*FNCompressFree)(LPVOID Address);
typedef DWORD (*FNCompressOpen)( DWORD dwParam1, DWORD MaxOrigSize, FNCompressAlloc AllocFn, FNCompressFree FreeFn, DWORD dwUnknown);
typedef DWORD (*FNCompressConvert)( DWORD ConvertStream, LPVOID CompAdr, DWORD CompSize, LPCVOID OrigAdr, DWORD OrigSize); 
typedef VOID (*FNCompressClose)( DWORD ConvertStream);

bool test_cecomp(const char *dllname)
{
    HMODULE hDll= LoadLibrary(dllname);
    if (hDll==NULL) {
        printf("ERROR - loadlib: %08x\n", GetLastError());
        return false;
    }
    CECOMPRESS comp= reinterpret_cast<CECOMPRESS>(GetProcAddress(hDll, "CECompress"));
    if (comp==NULL) {
        printf("ERROR - getproc(comp): %08x\n", GetLastError());
        if (!FreeLibrary(hDll))
            printf("ERROR - freelib: %08x\n", GetLastError());
        return false;
    }
    CEDECOMPRESS decomp= reinterpret_cast<CEDECOMPRESS>(GetProcAddress(hDll, "CEDecompress"));
    if (decomp==NULL) {
        printf("ERROR - getproc(decomp): %08x\n", GetLastError());
        if (!FreeLibrary(hDll))
            printf("ERROR - freelib: %08x\n", GetLastError());
        return false;
    }

    uint8_t teststring[512];
    for (unsigned i=0 ; i<512 ; i++)
        teststring[i]= i*i;
    int len= 512;
    uint8_t compstr[512];
    DWORD compres= comp(teststring, len, compstr, len-1, 1, 4096);
    if (compres==0xFFFFFFFF) {
        printf("comp error\n");
        return false;
    }
    printf("comp: %d chars\n", compres);
    uint8_t decompstr[512];
    DWORD decompres= decomp(compstr, compres, decompstr, len, 0, 1, 4096);
    if (decompres==0xFFFFFFFF) {
        printf("decomp error\n");
    }
    printf("decomp: %d chars\n", decompres);

    if (!FreeLibrary(hDll)) {
        printf("ERROR - freelib: %08x\n", GetLastError());
        return false;
    }
    return true;
}

typedef std::vector<uint8_t> ByteVector;
bool test_ntcomp(const char *dllname, const std::string& alg, const std::string& dir, const std::string& enc, ByteVector& in, ByteVector &out)
{
    HMODULE hDll= LoadLibrary(dllname);
    if (hDll==NULL) {
        printf("ERROR - loadlib: %08x\n", GetLastError());
        return false;
    }
    FNCompressOpen copen= reinterpret_cast<FNCompressOpen>(GetProcAddress(hDll, (alg+"_"+dir+"Open").c_str()));
    if (copen==NULL) {
        printf("ERROR - getproc(copen): %08x\n", GetLastError());
        if (!FreeLibrary(hDll))
            printf("ERROR - freelib: %08x\n", GetLastError());
        return false;
    }
    FNCompressConvert ccv= reinterpret_cast<FNCompressConvert>(GetProcAddress(hDll, (alg+"_"+dir+enc).c_str()));
    if (ccv==NULL) {
        printf("ERROR - getproc(ccv): %08x\n", GetLastError());
        if (!FreeLibrary(hDll))
            printf("ERROR - freelib: %08x\n", GetLastError());
        return false;
    }
    FNCompressClose cclose= reinterpret_cast<FNCompressClose>(GetProcAddress(hDll, (alg+"_"+dir+"Close").c_str()));
    if (cclose==NULL) {
        printf("ERROR - getproc(cclose): %08x\n", GetLastError());
        if (!FreeLibrary(hDll))
            printf("ERROR - freelib: %08x\n", GetLastError());
        return false;
    }
    printf("calling %s\n", (alg+"_"+dir+"Open").c_str());
    DWORD stream = copen(0x10000, 0x2000, FNCompressAlloc(malloc), FNCompressFree(free), 0);
    if (stream==0 || stream==0xFFFFFFFF) {
        printf("ERROR - copen: %08x\n", GetLastError());
        if (!FreeLibrary(hDll))
            printf("ERROR - freelib: %08x\n", GetLastError());
        return false;
    }

    DWORD cvres= ccv(stream, &out[0], out.size(), &in[0], in.size());
    cclose(stream);
    if (cvres==0 || cvres==0xFFFFFFFF) {
        printf("ERROR - copen: %08x\n", GetLastError());
        if (!FreeLibrary(hDll))
            printf("ERROR - freelib: %08x\n", GetLastError());
        return false;
    }
    printf("ccv: %08x\n", cvres);
    if (!FreeLibrary(hDll)) {
        printf("ERROR - freelib: %08x\n", GetLastError());
        return false;
    }
    return true;
}
int main(int argc, char **argv)
{
    //printf("v3 test: %sok\n", test_cecomp("CECompressv3.dll") ? "" : "not ");
    //printf("v4 test: %sok\n", test_cecomp("CECompressv4.dll") ? "" : "not ");
    const char* algorithm[]= { "LZX", "XPR" };
    const char* direction[]= { "Compress", "Decompress" };
    const char* enc[]= { "Encode", "Decode" };
    for (int ia=0 ; ia<2 ; ia++) {
        std::vector<ByteVector> v(3);
        v[0].resize(512);
        v[1].resize(512);
        v[2].resize(512);
        for (unsigned i=0 ; i<512 ; i++)
            v[0][i]=i*i;
        for (int id=0 ; id<2 ; id++) {
            printf("nt test: %s %s %sok\n", algorithm[ia], direction[id], test_ntcomp("cecompr_nt.dll", algorithm[ia], direction[id], enc[id], v[id], v[id+1]) ? "" : "not ");
        }
        printf("\n\n ... %lu\n\n", v[2].size());
    }
}
