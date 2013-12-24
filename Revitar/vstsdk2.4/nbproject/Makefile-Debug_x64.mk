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
	${OBJECTDIR}/public.sdk/source/vst2.x/audioeffect.o \
	${OBJECTDIR}/public.sdk/source/vst2.x/audioeffectx.o \
	${OBJECTDIR}/public.sdk/source/vst2.x/vstplugmain.o \
	${OBJECTDIR}/vstgui.sf/vstgui/aeffguieditor.o \
	${OBJECTDIR}/vstgui.sf/vstgui/cfileselector.o \
	${OBJECTDIR}/vstgui.sf/vstgui/cscrollview.o \
	${OBJECTDIR}/vstgui.sf/vstgui/ctabview.o \
	${OBJECTDIR}/vstgui.sf/vstgui/vstcontrols.o \
	${OBJECTDIR}/vstgui.sf/vstgui/vstgui.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m64
CXXFLAGS=-m64

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=--64

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvstsdk2.4.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvstsdk2.4.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvstsdk2.4.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvstsdk2.4.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvstsdk2.4.a

${OBJECTDIR}/public.sdk/source/vst2.x/audioeffect.o: public.sdk/source/vst2.x/audioeffect.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst2.x
	${RM} $@.d
	$(COMPILE.cc) -g -D_DEBUG -I. -Ipublic.sdk/source/vst2.x -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst2.x/audioeffect.o public.sdk/source/vst2.x/audioeffect.cpp

${OBJECTDIR}/public.sdk/source/vst2.x/audioeffectx.o: public.sdk/source/vst2.x/audioeffectx.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst2.x
	${RM} $@.d
	$(COMPILE.cc) -g -D_DEBUG -I. -Ipublic.sdk/source/vst2.x -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst2.x/audioeffectx.o public.sdk/source/vst2.x/audioeffectx.cpp

${OBJECTDIR}/public.sdk/source/vst2.x/vstplugmain.o: public.sdk/source/vst2.x/vstplugmain.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst2.x
	${RM} $@.d
	$(COMPILE.cc) -g -D_DEBUG -I. -Ipublic.sdk/source/vst2.x -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst2.x/vstplugmain.o public.sdk/source/vst2.x/vstplugmain.cpp

${OBJECTDIR}/vstgui.sf/vstgui/aeffguieditor.o: vstgui.sf/vstgui/aeffguieditor.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui.sf/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -D_DEBUG -I. -Ipublic.sdk/source/vst2.x -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui.sf/vstgui/aeffguieditor.o vstgui.sf/vstgui/aeffguieditor.cpp

${OBJECTDIR}/vstgui.sf/vstgui/cfileselector.o: vstgui.sf/vstgui/cfileselector.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui.sf/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -D_DEBUG -I. -Ipublic.sdk/source/vst2.x -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui.sf/vstgui/cfileselector.o vstgui.sf/vstgui/cfileselector.cpp

${OBJECTDIR}/vstgui.sf/vstgui/cscrollview.o: vstgui.sf/vstgui/cscrollview.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui.sf/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -D_DEBUG -I. -Ipublic.sdk/source/vst2.x -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui.sf/vstgui/cscrollview.o vstgui.sf/vstgui/cscrollview.cpp

${OBJECTDIR}/vstgui.sf/vstgui/ctabview.o: vstgui.sf/vstgui/ctabview.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui.sf/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -D_DEBUG -I. -Ipublic.sdk/source/vst2.x -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui.sf/vstgui/ctabview.o vstgui.sf/vstgui/ctabview.cpp

${OBJECTDIR}/vstgui.sf/vstgui/vstcontrols.o: vstgui.sf/vstgui/vstcontrols.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui.sf/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -D_DEBUG -I. -Ipublic.sdk/source/vst2.x -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui.sf/vstgui/vstcontrols.o vstgui.sf/vstgui/vstcontrols.cpp

${OBJECTDIR}/vstgui.sf/vstgui/vstgui.o: vstgui.sf/vstgui/vstgui.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui.sf/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -D_DEBUG -I. -Ipublic.sdk/source/vst2.x -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui.sf/vstgui/vstgui.o vstgui.sf/vstgui/vstgui.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvstsdk2.4.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
