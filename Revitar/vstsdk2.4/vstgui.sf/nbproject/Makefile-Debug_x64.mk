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
CND_PLATFORM=MinGW+w64-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug_x64
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/vstgui/aeffguieditor.o \
	${OBJECTDIR}/vstgui/cfileselector.o \
	${OBJECTDIR}/vstgui/cscrollview.o \
	${OBJECTDIR}/vstgui/ctabview.o \
	${OBJECTDIR}/vstgui/vstcontrols.o \
	${OBJECTDIR}/vstgui/vstgui.o


# C Compiler Flags
CFLAGS=-m64

# CC Compiler Flags
CCFLAGS=-m64
CXXFLAGS=-m64

# Fortran Compiler Flags
FFLAGS=-m64

# Assembler Flags
ASFLAGS=--64

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvstgui-3.0.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvstgui-3.0.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvstgui-3.0.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvstgui-3.0.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvstgui-3.0.a

${OBJECTDIR}/vstgui/aeffguieditor.o: vstgui/aeffguieditor.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=0 -D_DEBUG -Ivstgui -I.. -I../public.sdk/source/vst2.x -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui/aeffguieditor.o vstgui/aeffguieditor.cpp

${OBJECTDIR}/vstgui/cfileselector.o: vstgui/cfileselector.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=0 -D_DEBUG -Ivstgui -I.. -I../public.sdk/source/vst2.x -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui/cfileselector.o vstgui/cfileselector.cpp

${OBJECTDIR}/vstgui/cscrollview.o: vstgui/cscrollview.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=0 -D_DEBUG -Ivstgui -I.. -I../public.sdk/source/vst2.x -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui/cscrollview.o vstgui/cscrollview.cpp

${OBJECTDIR}/vstgui/ctabview.o: vstgui/ctabview.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=0 -D_DEBUG -Ivstgui -I.. -I../public.sdk/source/vst2.x -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui/ctabview.o vstgui/ctabview.cpp

${OBJECTDIR}/vstgui/vstcontrols.o: vstgui/vstcontrols.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=0 -D_DEBUG -Ivstgui -I.. -I../public.sdk/source/vst2.x -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui/vstcontrols.o vstgui/vstcontrols.cpp

${OBJECTDIR}/vstgui/vstgui.o: vstgui/vstgui.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=0 -D_DEBUG -Ivstgui -I.. -I../public.sdk/source/vst2.x -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui/vstgui.o vstgui/vstgui.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvstgui-3.0.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
