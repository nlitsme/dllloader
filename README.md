dllloader
=========

A C++ file which helps using a win32 DLL directly on linux or MacOS.
The interface mimics the LoadLibrary/GetProcAddress interface from windows.

Note: this is an old project, this was useful in the time i was still working on Windows CE.

This will no longer work in MacOS 10.15 because 32-bit support will be dropped.
Also, i have not been able to build this since 10.14, since 32-bit libraries no longer ship
with Xcode.


Author
======

Willem Hengeveld <itsme@xs4all.nl>

