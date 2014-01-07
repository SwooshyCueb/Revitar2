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
CND_CONF=Release_x64_VST2
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
	${OBJECTDIR}/public.sdk/source/vst2.x/vstplugmain.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvst-2.4.3.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvst-2.4.3.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvst-2.4.3.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvst-2.4.3.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvst-2.4.3.a

${OBJECTDIR}/public.sdk/source/vst2.x/audioeffect.o: public.sdk/source/vst2.x/audioeffect.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst2.x
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -DNDEBUG -DRELEASE=1 -I. -Ipublic.sdk/source/vst2.x -Ivstgui.sf -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/public.sdk/source/vst2.x/audioeffect.o public.sdk/source/vst2.x/audioeffect.cpp

${OBJECTDIR}/public.sdk/source/vst2.x/audioeffectx.o: public.sdk/source/vst2.x/audioeffectx.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst2.x
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -DNDEBUG -DRELEASE=1 -I. -Ipublic.sdk/source/vst2.x -Ivstgui.sf -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/public.sdk/source/vst2.x/audioeffectx.o public.sdk/source/vst2.x/audioeffectx.cpp

${OBJECTDIR}/public.sdk/source/vst2.x/vstplugmain.o: public.sdk/source/vst2.x/vstplugmain.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst2.x
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -s -DNDEBUG -DRELEASE=1 -I. -Ipublic.sdk/source/vst2.x -Ivstgui.sf -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/public.sdk/source/vst2.x/vstplugmain.o public.sdk/source/vst2.x/vstplugmain.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvst-2.4.3.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
