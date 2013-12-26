Revitar2
========

SwooshyCueb's fork of Revitar 2.
Revitar 2 created by C. Lawrence Zitnick III.

Released under the Mozilla Public License 2.0 with permission from the original
author.

You will need mingw (and msys if on Windows) to compile this. I used these
toolchains:
x86: http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/4.8.2/threads-win32/sjlj/i686-4.8.2-release-win32-sjlj-rt_v3-rev1.7z/download
x64: http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/4.8.2/threads-win32/sjlj/x86_64-4.8.2-release-win32-sjlj-rt_v3-rev1.7z/download

Currently only VST 2.4 and VSTGUI 3.0 are supported. You can grab these here:
http://www.dith.it/listing/vst_stuff/vst_sdk2_4_rev2.zip
Just extract the vstsdk2.4 folder into the project directory.

The VSTGUI3.6 configurations are currently a work in progress. If you want to
play around with the VSTGUI3.6 code, download the VST SDK 3.6.0 from Steinberg
and stick it in the vst3sdk directory.

Currently only the release configurations are set to link libstdc++ and libgcc
statically.

Please note that the x64 builds have not been tested thoroughly.