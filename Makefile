ifneq ($(wildcard /sbin/bsdlabel),)
OSTYPE=freebsd
endif
ifneq ($(wildcard /System/Library/Extensions/AppleFileSystemDriver.kext),)
OSTYPE=darwin
endif
ifneq ($(wildcard /sbin/modprobe),)
OSTYPE=linux
endif
ifneq ($(wildcard $(SystemRoot)/explorer.exe),)
OSTYPE=windows
endif

# gcc on osx:
# -mstackrealign  is needed to fix the '__dyld_misaligned_stack_error' issue.

msccdefs+=/D _CRT_SECURE_NO_WARNINGS /D _SECURE_SCL=0 /D _HAS_ITERATOR_DEBUGGING=0
ifeq ($(HOSTTYPE),amd64)
CFLAGS=-m32 -isystem /usr/include/32bit
else
ifeq ($(OSTYPE),darwin)
CFLAGS=-mstackrealign -m32
else
endif
endif

CFLAGS+=-I../common

all: tstcompr tstload

clean:
	$(RM) -f $(wildcard *.pdb *.exe *.obj *.o *.ilk a.out) tstcompr tstload

tstcompr: dllloader.cpp tstcompr.cpp
ifeq ($(VSTUDNET),)
	g++ $(CFLAGS) -Wall -g dllloader.cpp tstcompr.cpp -o $@
else
	cl /EHsc $(msccdefs) /Zi tstcompr.cpp dllloader.cpp /I "$(VSTUDNET)\vc\include" /I ../include/msvc  /link /libpath:"$(VSTUDNET)\vc\lib"
	cl /D_USE_WINDOWS /EHsc $(msccdefs) /Zi tstcompr.cpp dllloader.cpp /I "$(VSTUDNET)\vc\include" /I "$(VSTUDNET)\vc\platformsdk\include" /I ../include/msvc /Fe"tstloader2.exe" /link /libpath:"$(VSTUDNET)\vc\lib" /libpath:"$(VSTUDNET)\vc\platformsdk\lib"
endif


tstload: dllloader.cpp tstload.cpp
ifeq ($(VSTUDNET),)
	g++ $(CFLAGS) -Wall -g dllloader.cpp tstload.cpp -o $@
else
	cl /EHsc $(msccdefs) /Zi tstload.cpp dllloader.cpp /I "$(VSTUDNET)\vc\include" /I ../include/msvc  /link /libpath:"$(VSTUDNET)\vc\lib"
	cl /D_USE_WINDOWS /EHsc $(msccdefs) /Zi tstload.cpp dllloader.cpp /I "$(VSTUDNET)\vc\include" /I "$(VSTUDNET)\vc\platformsdk\include" /I ../include/msvc /Fe"tstloader2.exe" /link /libpath:"$(VSTUDNET)\vc\lib" /libpath:"$(VSTUDNET)\vc\platformsdk\lib"
endif

