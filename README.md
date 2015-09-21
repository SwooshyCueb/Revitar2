Revitar2
========

Revitar 2 was originally created by C. Lawrence Zitnick III.  
This fork is maintained by Markus Kitsinger (SwooshyCueb)

Released under the Mozilla Public License 2.0 with permission from the original
author.

You will need mingw (and msys if on Windows) to compile this. I used these
toolchains:  
x86: http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/4.8.5/threads-win32/dwarf/i686-4.8.5-release-win32-dwarf-rt_v4-rev0.7z/download  
x64: http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/4.8.5/threads-win32/seh/x86_64-4.8.5-release-win32-seh-rt_v4-rev0.7z/download  
(Yes, I know, SourceForge is evil. Thankfully, they don't seem to be messing
with the mingw folks.)

Currently only VST 2.4 and VSTGUI 3.0 are supported. You can grab these here:  
http://www.dith.it/listing/vst_stuff/vst_sdk2_4_rev2.zip  
Just extract the vstsdk2.4 folder into the project directory.  
Revitar will compile against VSTGUI 3.6, but the UI will not render properly.
