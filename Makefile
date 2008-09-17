
# gcc on osx:
# -mstackrealign  is needed to fix the '__dyld_misaligned_stack_error' issue.

msccdefs+=/D _CRT_SECURE_NO_WARNINGS /D _SECURE_SCL=0 /D _HAS_ITERATOR_DEBUGGING=0
all:
ifeq ($(VSTUDNET),)
	g++ -mstackrealign -Wall -g dllloader.cpp tstloader.cpp
else
	cl /EHsc $(msccdefs) /Zi tstloader.cpp dllloader.cpp /I "$(VSTUDNET)\vc\include" /I ../include/msvc  /link /libpath:"$(VSTUDNET)\vc\lib"
	cl /D_USE_WINDOWS /EHsc $(msccdefs) /Zi tstloader.cpp dllloader.cpp /I "$(VSTUDNET)\vc\include" /I "$(VSTUDNET)\vc\platformsdk\include" /I ../include/msvc /Fe"tstloader2.exe" /link /libpath:"$(VSTUDNET)\vc\lib" /libpath:"$(VSTUDNET)\vc\platformsdk\lib"
endif
