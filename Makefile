ifneq ($(wildcard /sbin/bsdlabel),)
OSTYPE=freebsd
endif
ifneq ($(wildcard /System/Library/Extensions/AppleFileSystemDriver.kext),)
OSTYPE=darwin
endif
ifneq ($(wildcard /sbin/modprobe),)
OSTYPE=linux
endif
ifneq ($(wildcard $(SystemRoot)/explorer.exe $(SYSTEMROOT)/explorer.exe),)
OSTYPE=windows
endif

# gcc on osx:
# -mstackrealign  is needed to fix the '__dyld_misaligned_stack_error' issue.

msccdefs+=/D _CRT_SECURE_NO_WARNINGS /D _SECURE_SCL=0 /D _HAS_ITERATOR_DEBUGGING=0 /D NOMINMAX
ifeq ($(HOSTTYPE),amd64)
CFLAGS=-m32 -isystem /usr/include/32bit
else
ifeq ($(OSTYPE),darwin)
CFLAGS=-mstackrealign -m32
else
endif
endif

CFLAGS+=-I../common -I /opt/local/include
SDKINCS=/I "$(VStudNet)\vc\include" /I "$(VStudNet)\vc\platformsdk\include"

all: tstcompr tstload

clean:
	$(RM) -f $(wildcard *.pdb *.exe *.obj *.o *.ilk a.out) tstcompr tstload
	$(RM) -r $(wildcard *.dSYM)

tstcompr: dllloader.cpp tstcompr.cpp
ifeq ($(VStudNet),)
	g++ $(CFLAGS) -Wall -g $^ -o $@
else
	cl /I ../common /EHsc $(msccdefs) /Zi $^ $(SDKINCS) /I ../include/msvc  /link /libpath:"$(VStudNet)\vc\lib"
	cl /I ../common /D_USE_WINDOWS /EHsc $(msccdefs) /Zi $^ $(SDKINCS) /I ../include/msvc /Fe"tstloader2.exe" /link /libpath:"$(VStudNet)\vc\lib" /libpath:"$(VStudNet)\vc\platformsdk\lib"
endif

tstload: dllloader.cpp tstload.cpp
ifeq ($(VStudNet),)
	g++ $(CFLAGS) -Wall -g $^ -o $@
else
	cl /I ../common /EHsc $(msccdefs) /Zi $^ $(SDKINCS) /I ../include/msvc  /link /libpath:"$(VStudNet)\vc\lib"
	cl /I ../common /D_USE_WINDOWS /EHsc $(msccdefs) /Zi $^ $(SDKINCS) /I ../include/msvc /Fe"tstloader2.exe" /link /libpath:"$(VStudNet)\vc\lib" /libpath:"$(VStudNet)\vc\platformsdk\lib"
endif

