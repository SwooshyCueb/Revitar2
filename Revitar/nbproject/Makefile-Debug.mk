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
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
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
CFLAGS=-fpermissive

# CC Compiler Flags
CCFLAGS=-fpermissive
CXXFLAGS=-fpermissive

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=../vstsdk2.4.2/dist/Release/MinGW-Windows/libvstsdk2.4.2.a ../vstgui/dist/Release/MinGW-Windows/libvstgui.a -mwindows

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librevitarfork.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librevitarfork.${CND_DLIB_EXT}: ../vstsdk2.4.2/dist/Release/MinGW-Windows/libvstsdk2.4.2.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librevitarfork.${CND_DLIB_EXT}: ../vstgui/dist/Release/MinGW-Windows/libvstgui.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librevitarfork.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librevitarfork.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -lole32 -lkernel32 -lgdi32 -luuid -luser32 --def Revitar.def -shared

${OBJECTDIR}/RevEditor.o: RevEditor.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -w -DBOOST_THREAD_USE_LIB -DREVITAR_2_01_EXPORTS -DUSE_LIBPNG=0 -DVSTi -DWIN32 -DWINDOWS=1 -D_CRT_SECURE_NO_WARNINGS -D_USRDLL -D_WINDOWS -fpermissive  -MMD -MP -MF $@.d -o ${OBJECTDIR}/RevEditor.o RevEditor.cpp

${OBJECTDIR}/Revitar.o: Revitar.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -w -DBOOST_THREAD_USE_LIB -DREVITAR_2_01_EXPORTS -DUSE_LIBPNG=0 -DVSTi -DWIN32 -DWINDOWS=1 -D_CRT_SECURE_NO_WARNINGS -D_USRDLL -D_WINDOWS -fpermissive  -MMD -MP -MF $@.d -o ${OBJECTDIR}/Revitar.o Revitar.cpp

${OBJECTDIR}/displayScreen.o: displayScreen.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -w -DBOOST_THREAD_USE_LIB -DREVITAR_2_01_EXPORTS -DUSE_LIBPNG=0 -DVSTi -DWIN32 -DWINDOWS=1 -D_CRT_SECURE_NO_WARNINGS -D_USRDLL -D_WINDOWS -fpermissive  -MMD -MP -MF $@.d -o ${OBJECTDIR}/displayScreen.o displayScreen.cpp

${OBJECTDIR}/presets.o: presets.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -w -DBOOST_THREAD_USE_LIB -DREVITAR_2_01_EXPORTS -DUSE_LIBPNG=0 -DVSTi -DWIN32 -DWINDOWS=1 -D_CRT_SECURE_NO_WARNINGS -D_USRDLL -D_WINDOWS -fpermissive  -MMD -MP -MF $@.d -o ${OBJECTDIR}/presets.o presets.cpp

# Subprojects
.build-subprojects:
	cd ../vstsdk2.4.2 && ${MAKE}  -f Makefile CONF=Release
	cd ../vstgui && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librevitarfork.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:
	cd ../vstsdk2.4.2 && ${MAKE}  -f Makefile CONF=Release clean
	cd ../vstgui && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
