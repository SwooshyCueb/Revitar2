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
CND_CONF=Release_x64
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/vstgui/aeffguieditor.o \
	${OBJECTDIR}/vstgui/cdatabrowser.o \
	${OBJECTDIR}/vstgui/cfileselector.o \
	${OBJECTDIR}/vstgui/cscrollview.o \
	${OBJECTDIR}/vstgui/ctabview.o \
	${OBJECTDIR}/vstgui/ctooltipsupport.o \
	${OBJECTDIR}/vstgui/cvstguitimer.o \
	${OBJECTDIR}/vstgui/plugguieditor.o \
	${OBJECTDIR}/vstgui/vstcontrols.o \
	${OBJECTDIR}/vstgui/vstgui.o \
	${OBJECTDIR}/vstgui/vstguidebug.o \
	${OBJECTDIR}/vstgui/winfileselector.o


# C Compiler Flags
CFLAGS=-m64

# CC Compiler Flags
CCFLAGS=-m64
CXXFLAGS=-m64

# Fortran Compiler Flags
FFLAGS=-s -m64

# Assembler Flags
ASFLAGS=--64

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvstgui-3.6.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvstgui-3.6.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvstgui-3.6.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvstgui-3.6.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvstgui-3.6.a

${OBJECTDIR}/vstgui/aeffguieditor.o: vstgui/aeffguieditor.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -DNDEBUG -DRELEASE=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=0 -Ivstgui -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui/aeffguieditor.o vstgui/aeffguieditor.cpp

${OBJECTDIR}/vstgui/cdatabrowser.o: vstgui/cdatabrowser.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -DNDEBUG -DRELEASE=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=0 -Ivstgui -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui/cdatabrowser.o vstgui/cdatabrowser.cpp

${OBJECTDIR}/vstgui/cfileselector.o: vstgui/cfileselector.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -DNDEBUG -DRELEASE=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=0 -Ivstgui -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui/cfileselector.o vstgui/cfileselector.cpp

${OBJECTDIR}/vstgui/cscrollview.o: vstgui/cscrollview.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -DNDEBUG -DRELEASE=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=0 -Ivstgui -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui/cscrollview.o vstgui/cscrollview.cpp

${OBJECTDIR}/vstgui/ctabview.o: vstgui/ctabview.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -DNDEBUG -DRELEASE=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=0 -Ivstgui -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui/ctabview.o vstgui/ctabview.cpp

${OBJECTDIR}/vstgui/ctooltipsupport.o: vstgui/ctooltipsupport.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -DNDEBUG -DRELEASE=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=0 -Ivstgui -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui/ctooltipsupport.o vstgui/ctooltipsupport.cpp

${OBJECTDIR}/vstgui/cvstguitimer.o: vstgui/cvstguitimer.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -DNDEBUG -DRELEASE=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=0 -Ivstgui -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui/cvstguitimer.o vstgui/cvstguitimer.cpp

${OBJECTDIR}/vstgui/plugguieditor.o: vstgui/plugguieditor.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -DNDEBUG -DRELEASE=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=0 -Ivstgui -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui/plugguieditor.o vstgui/plugguieditor.cpp

${OBJECTDIR}/vstgui/vstcontrols.o: vstgui/vstcontrols.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -DNDEBUG -DRELEASE=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=0 -Ivstgui -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui/vstcontrols.o vstgui/vstcontrols.cpp

${OBJECTDIR}/vstgui/vstgui.o: vstgui/vstgui.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -DNDEBUG -DRELEASE=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=0 -Ivstgui -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui/vstgui.o vstgui/vstgui.cpp

${OBJECTDIR}/vstgui/vstguidebug.o: vstgui/vstguidebug.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -DNDEBUG -DRELEASE=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=0 -Ivstgui -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui/vstguidebug.o vstgui/vstguidebug.cpp

${OBJECTDIR}/vstgui/winfileselector.o: vstgui/winfileselector.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui
	${RM} $@.d
	$(COMPILE.cc) -O3 -s -DNDEBUG -DRELEASE=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=0 -Ivstgui -I.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui/winfileselector.o vstgui/winfileselector.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvstgui-3.6.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
