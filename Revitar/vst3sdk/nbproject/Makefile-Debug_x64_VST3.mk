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
CND_CONF=Debug_x64_VST3
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/public.sdk/source/vst/vstaudioeffect.o \
	${OBJECTDIR}/public.sdk/source/vst/vstbus.o \
	${OBJECTDIR}/public.sdk/source/vst/vstcomponent.o \
	${OBJECTDIR}/public.sdk/source/vst/vstcomponentbase.o \
	${OBJECTDIR}/public.sdk/source/vst/vsteditcontroller.o \
	${OBJECTDIR}/public.sdk/source/vst/vstguieditor.o \
	${OBJECTDIR}/public.sdk/source/vst/vstinitiids.o \
	${OBJECTDIR}/public.sdk/source/vst/vstnoteexpressiontypes.o \
	${OBJECTDIR}/public.sdk/source/vst/vstparameters.o \
	${OBJECTDIR}/public.sdk/source/vst/vstpresetfile.o \
	${OBJECTDIR}/public.sdk/source/vst/vstrepresentation.o \
	${OBJECTDIR}/public.sdk/source/vst/vstsinglecomponenteffect.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvst3sdk.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvst3sdk.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvst3sdk.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvst3sdk.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvst3sdk.a

${OBJECTDIR}/public.sdk/source/vst/vstaudioeffect.o: public.sdk/source/vst/vstaudioeffect.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst/vstaudioeffect.o public.sdk/source/vst/vstaudioeffect.cpp

${OBJECTDIR}/public.sdk/source/vst/vstbus.o: public.sdk/source/vst/vstbus.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst/vstbus.o public.sdk/source/vst/vstbus.cpp

${OBJECTDIR}/public.sdk/source/vst/vstcomponent.o: public.sdk/source/vst/vstcomponent.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst/vstcomponent.o public.sdk/source/vst/vstcomponent.cpp

${OBJECTDIR}/public.sdk/source/vst/vstcomponentbase.o: public.sdk/source/vst/vstcomponentbase.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst/vstcomponentbase.o public.sdk/source/vst/vstcomponentbase.cpp

${OBJECTDIR}/public.sdk/source/vst/vsteditcontroller.o: public.sdk/source/vst/vsteditcontroller.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst/vsteditcontroller.o public.sdk/source/vst/vsteditcontroller.cpp

${OBJECTDIR}/public.sdk/source/vst/vstguieditor.o: public.sdk/source/vst/vstguieditor.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst/vstguieditor.o public.sdk/source/vst/vstguieditor.cpp

${OBJECTDIR}/public.sdk/source/vst/vstinitiids.o: public.sdk/source/vst/vstinitiids.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst/vstinitiids.o public.sdk/source/vst/vstinitiids.cpp

${OBJECTDIR}/public.sdk/source/vst/vstnoteexpressiontypes.o: public.sdk/source/vst/vstnoteexpressiontypes.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst/vstnoteexpressiontypes.o public.sdk/source/vst/vstnoteexpressiontypes.cpp

${OBJECTDIR}/public.sdk/source/vst/vstparameters.o: public.sdk/source/vst/vstparameters.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst/vstparameters.o public.sdk/source/vst/vstparameters.cpp

${OBJECTDIR}/public.sdk/source/vst/vstpresetfile.o: public.sdk/source/vst/vstpresetfile.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst/vstpresetfile.o public.sdk/source/vst/vstpresetfile.cpp

${OBJECTDIR}/public.sdk/source/vst/vstrepresentation.o: public.sdk/source/vst/vstrepresentation.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst/vstrepresentation.o public.sdk/source/vst/vstrepresentation.cpp

${OBJECTDIR}/public.sdk/source/vst/vstsinglecomponenteffect.o: public.sdk/source/vst/vstsinglecomponenteffect.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst/vstsinglecomponenteffect.o public.sdk/source/vst/vstsinglecomponenteffect.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvst3sdk.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
