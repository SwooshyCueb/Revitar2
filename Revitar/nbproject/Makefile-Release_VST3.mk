#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW+-Windows
CND_DLIB_EXT=dll
CND_CONF=Release_VST3
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/RevEditor.o \
	${OBJECTDIR}/Revitar.o \
	${OBJECTDIR}/displayScreen.o \
	${OBJECTDIR}/presets.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m32 -fpermissive
CXXFLAGS=-m32 -fpermissive

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=vst3sdk/dist/Release/MinGW+-Windows/libvst3sdk.a rcf.o -mwindows

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/Revitar2.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/Revitar2.${CND_DLIB_EXT}: vst3sdk/dist/Release/MinGW+-Windows/libvst3sdk.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/Revitar2.${CND_DLIB_EXT}: rcf.o

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/Revitar2.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/Revitar2.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -lole32 -lkernel32 -lgdi32 -luuid -luser32 --def Revitar.def -static-libstdc++ -static-libgcc -shared

${OBJECTDIR}/RevEditor.o: RevEditor.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -w -DBOOST_THREAD_USE_LIB -DREVITAR_2_01_EXPORTS -DUSE_LIBPNG=0 -DVSTi -DWIN32 -DWINDOWS=1 -D_CRT_SECURE_NO_WARNINGS -D_USRDLL -D_WINDOWS -DVST3 -DNDEBUG -DVSTGUI_ENABLE_DEPRECATED_METHODS -Ivst3sdk -Ivst3sdk/public.sdk/source/vst -Ivst3sdk/vstgui.sf -fpermissive  -MMD -MP -MF $@.d -o ${OBJECTDIR}/RevEditor.o RevEditor.cpp

${OBJECTDIR}/Revitar.o: Revitar.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -w -DBOOST_THREAD_USE_LIB -DREVITAR_2_01_EXPORTS -DUSE_LIBPNG=0 -DVSTi -DWIN32 -DWINDOWS=1 -D_CRT_SECURE_NO_WARNINGS -D_USRDLL -D_WINDOWS -DVST3 -DNDEBUG -DVSTGUI_ENABLE_DEPRECATED_METHODS -Ivst3sdk -Ivst3sdk/public.sdk/source/vst -Ivst3sdk/vstgui.sf -fpermissive  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Revitar.o Revitar.cpp

${OBJECTDIR}/displayScreen.o: displayScreen.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -w -DBOOST_THREAD_USE_LIB -DREVITAR_2_01_EXPORTS -DUSE_LIBPNG=0 -DVSTi -DWIN32 -DWINDOWS=1 -D_CRT_SECURE_NO_WARNINGS -D_USRDLL -D_WINDOWS -DVST3 -DNDEBUG -DVSTGUI_ENABLE_DEPRECATED_METHODS -Ivst3sdk -Ivst3sdk/public.sdk/source/vst -Ivst3sdk/vstgui.sf -fpermissive  -MMD -MP -MF $@.d -o ${OBJECTDIR}/displayScreen.o displayScreen.cpp

${OBJECTDIR}/presets.o: presets.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -w -DBOOST_THREAD_USE_LIB -DREVITAR_2_01_EXPORTS -DUSE_LIBPNG=0 -DVSTi -DWIN32 -DWINDOWS=1 -D_CRT_SECURE_NO_WARNINGS -D_USRDLL -D_WINDOWS -DVST3 -DNDEBUG -DVSTGUI_ENABLE_DEPRECATED_METHODS -Ivst3sdk -Ivst3sdk/public.sdk/source/vst -Ivst3sdk/vstgui.sf -fpermissive  -MMD -MP -MF $@.d -o ${OBJECTDIR}/presets.o presets.cpp

# Subprojects
.build-subprojects:
	cd vst3sdk && ${MAKE}  -f Makefile CONF=Release
	cd vst3sdk && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/Revitar2.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:
	cd vst3sdk && ${MAKE}  -f Makefile CONF=Release clean
	cd vst3sdk && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
