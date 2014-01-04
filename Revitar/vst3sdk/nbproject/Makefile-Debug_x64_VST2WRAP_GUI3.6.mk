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
CND_CONF=Debug_x64_VST2WRAP_GUI3.6
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/base/source/fatomic.o \
	${OBJECTDIR}/base/source/fcriticalperformance.o \
	${OBJECTDIR}/base/source/fdebug.o \
	${OBJECTDIR}/base/source/fdynlib.o \
	${OBJECTDIR}/base/source/fmemory.o \
	${OBJECTDIR}/base/source/fobject.o \
	${OBJECTDIR}/base/source/fstreamer.o \
	${OBJECTDIR}/base/source/fstring.o \
	${OBJECTDIR}/base/source/fthread.o \
	${OBJECTDIR}/base/source/timer.o \
	${OBJECTDIR}/pluginterfaces/base/conststringtable.o \
	${OBJECTDIR}/pluginterfaces/base/funknown.o \
	${OBJECTDIR}/public.sdk/source/common/memorystream.o \
	${OBJECTDIR}/public.sdk/source/common/pluginview.o \
	${OBJECTDIR}/public.sdk/source/vst/hosting/eventlist.o \
	${OBJECTDIR}/public.sdk/source/vst/hosting/hostclasses.o \
	${OBJECTDIR}/public.sdk/source/vst/hosting/parameterchanges.o \
	${OBJECTDIR}/public.sdk/source/vst/hosting/processdata.o \
	${OBJECTDIR}/public.sdk/source/vst/vst2wrapper/vst2wrapper.o \
	${OBJECTDIR}/public.sdk/source/vst2.x/audioeffect.o \
	${OBJECTDIR}/public.sdk/source/vst2.x/audioeffectx.o \
	${OBJECTDIR}/public.sdk/source/vst2.x/vstplugmain.o \
	${OBJECTDIR}/vstgui.sf/vstgui/aeffguieditor.o \
	${OBJECTDIR}/vstgui.sf/vstgui/cdatabrowser.o \
	${OBJECTDIR}/vstgui.sf/vstgui/cfileselector.o \
	${OBJECTDIR}/vstgui.sf/vstgui/cscrollview.o \
	${OBJECTDIR}/vstgui.sf/vstgui/ctabview.o \
	${OBJECTDIR}/vstgui.sf/vstgui/ctooltipsupport.o \
	${OBJECTDIR}/vstgui.sf/vstgui/cvstguitimer.o \
	${OBJECTDIR}/vstgui.sf/vstgui/plugguieditor.o \
	${OBJECTDIR}/vstgui.sf/vstgui/vstcontrols.o \
	${OBJECTDIR}/vstgui.sf/vstgui/vstgui.o \
	${OBJECTDIR}/vstgui.sf/vstgui/vstguidebug.o \
	${OBJECTDIR}/vstgui.sf/vstgui/winfileselector.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m64 -fpermissive
CXXFLAGS=-m64 -fpermissive

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

${OBJECTDIR}/base/source/fatomic.o: base/source/fatomic.cpp 
	${MKDIR} -p ${OBJECTDIR}/base/source
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/base/source/fatomic.o base/source/fatomic.cpp

${OBJECTDIR}/base/source/fcriticalperformance.o: base/source/fcriticalperformance.cpp 
	${MKDIR} -p ${OBJECTDIR}/base/source
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/base/source/fcriticalperformance.o base/source/fcriticalperformance.cpp

${OBJECTDIR}/base/source/fdebug.o: base/source/fdebug.cpp 
	${MKDIR} -p ${OBJECTDIR}/base/source
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/base/source/fdebug.o base/source/fdebug.cpp

${OBJECTDIR}/base/source/fdynlib.o: base/source/fdynlib.cpp 
	${MKDIR} -p ${OBJECTDIR}/base/source
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/base/source/fdynlib.o base/source/fdynlib.cpp

${OBJECTDIR}/base/source/fmemory.o: base/source/fmemory.cpp 
	${MKDIR} -p ${OBJECTDIR}/base/source
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/base/source/fmemory.o base/source/fmemory.cpp

${OBJECTDIR}/base/source/fobject.o: base/source/fobject.cpp 
	${MKDIR} -p ${OBJECTDIR}/base/source
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/base/source/fobject.o base/source/fobject.cpp

${OBJECTDIR}/base/source/fstreamer.o: base/source/fstreamer.cpp 
	${MKDIR} -p ${OBJECTDIR}/base/source
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/base/source/fstreamer.o base/source/fstreamer.cpp

${OBJECTDIR}/base/source/fstring.o: base/source/fstring.cpp 
	${MKDIR} -p ${OBJECTDIR}/base/source
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/base/source/fstring.o base/source/fstring.cpp

${OBJECTDIR}/base/source/fthread.o: base/source/fthread.cpp 
	${MKDIR} -p ${OBJECTDIR}/base/source
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/base/source/fthread.o base/source/fthread.cpp

${OBJECTDIR}/base/source/timer.o: base/source/timer.cpp 
	${MKDIR} -p ${OBJECTDIR}/base/source
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/base/source/timer.o base/source/timer.cpp

${OBJECTDIR}/pluginterfaces/base/conststringtable.o: pluginterfaces/base/conststringtable.cpp 
	${MKDIR} -p ${OBJECTDIR}/pluginterfaces/base
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/pluginterfaces/base/conststringtable.o pluginterfaces/base/conststringtable.cpp

${OBJECTDIR}/pluginterfaces/base/funknown.o: pluginterfaces/base/funknown.cpp 
	${MKDIR} -p ${OBJECTDIR}/pluginterfaces/base
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/pluginterfaces/base/funknown.o pluginterfaces/base/funknown.cpp

${OBJECTDIR}/public.sdk/source/common/memorystream.o: public.sdk/source/common/memorystream.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/common
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/common/memorystream.o public.sdk/source/common/memorystream.cpp

${OBJECTDIR}/public.sdk/source/common/pluginview.o: public.sdk/source/common/pluginview.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/common
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/common/pluginview.o public.sdk/source/common/pluginview.cpp

${OBJECTDIR}/public.sdk/source/vst/hosting/eventlist.o: public.sdk/source/vst/hosting/eventlist.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst/hosting
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst/hosting/eventlist.o public.sdk/source/vst/hosting/eventlist.cpp

${OBJECTDIR}/public.sdk/source/vst/hosting/hostclasses.o: public.sdk/source/vst/hosting/hostclasses.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst/hosting
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst/hosting/hostclasses.o public.sdk/source/vst/hosting/hostclasses.cpp

${OBJECTDIR}/public.sdk/source/vst/hosting/parameterchanges.o: public.sdk/source/vst/hosting/parameterchanges.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst/hosting
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst/hosting/parameterchanges.o public.sdk/source/vst/hosting/parameterchanges.cpp

${OBJECTDIR}/public.sdk/source/vst/hosting/processdata.o: public.sdk/source/vst/hosting/processdata.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst/hosting
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst/hosting/processdata.o public.sdk/source/vst/hosting/processdata.cpp

${OBJECTDIR}/public.sdk/source/vst/vst2wrapper/vst2wrapper.o: public.sdk/source/vst/vst2wrapper/vst2wrapper.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst/vst2wrapper
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst/vst2wrapper/vst2wrapper.o public.sdk/source/vst/vst2wrapper/vst2wrapper.cpp

${OBJECTDIR}/public.sdk/source/vst2.x/audioeffect.o: public.sdk/source/vst2.x/audioeffect.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst2.x
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst2.x/audioeffect.o public.sdk/source/vst2.x/audioeffect.cpp

${OBJECTDIR}/public.sdk/source/vst2.x/audioeffectx.o: public.sdk/source/vst2.x/audioeffectx.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst2.x
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst2.x/audioeffectx.o public.sdk/source/vst2.x/audioeffectx.cpp

${OBJECTDIR}/public.sdk/source/vst2.x/vstplugmain.o: public.sdk/source/vst2.x/vstplugmain.cpp 
	${MKDIR} -p ${OBJECTDIR}/public.sdk/source/vst2.x
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/public.sdk/source/vst2.x/vstplugmain.o public.sdk/source/vst2.x/vstplugmain.cpp

${OBJECTDIR}/vstgui.sf/vstgui/aeffguieditor.o: vstgui.sf/vstgui/aeffguieditor.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui.sf/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui.sf/vstgui/aeffguieditor.o vstgui.sf/vstgui/aeffguieditor.cpp

${OBJECTDIR}/vstgui.sf/vstgui/cdatabrowser.o: vstgui.sf/vstgui/cdatabrowser.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui.sf/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui.sf/vstgui/cdatabrowser.o vstgui.sf/vstgui/cdatabrowser.cpp

${OBJECTDIR}/vstgui.sf/vstgui/cfileselector.o: vstgui.sf/vstgui/cfileselector.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui.sf/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui.sf/vstgui/cfileselector.o vstgui.sf/vstgui/cfileselector.cpp

${OBJECTDIR}/vstgui.sf/vstgui/cscrollview.o: vstgui.sf/vstgui/cscrollview.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui.sf/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui.sf/vstgui/cscrollview.o vstgui.sf/vstgui/cscrollview.cpp

${OBJECTDIR}/vstgui.sf/vstgui/ctabview.o: vstgui.sf/vstgui/ctabview.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui.sf/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui.sf/vstgui/ctabview.o vstgui.sf/vstgui/ctabview.cpp

${OBJECTDIR}/vstgui.sf/vstgui/ctooltipsupport.o: vstgui.sf/vstgui/ctooltipsupport.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui.sf/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui.sf/vstgui/ctooltipsupport.o vstgui.sf/vstgui/ctooltipsupport.cpp

${OBJECTDIR}/vstgui.sf/vstgui/cvstguitimer.o: vstgui.sf/vstgui/cvstguitimer.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui.sf/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui.sf/vstgui/cvstguitimer.o vstgui.sf/vstgui/cvstguitimer.cpp

${OBJECTDIR}/vstgui.sf/vstgui/plugguieditor.o: vstgui.sf/vstgui/plugguieditor.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui.sf/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui.sf/vstgui/plugguieditor.o vstgui.sf/vstgui/plugguieditor.cpp

${OBJECTDIR}/vstgui.sf/vstgui/vstcontrols.o: vstgui.sf/vstgui/vstcontrols.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui.sf/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui.sf/vstgui/vstcontrols.o vstgui.sf/vstgui/vstcontrols.cpp

${OBJECTDIR}/vstgui.sf/vstgui/vstgui.o: vstgui.sf/vstgui/vstgui.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui.sf/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui.sf/vstgui/vstgui.o vstgui.sf/vstgui/vstgui.cpp

${OBJECTDIR}/vstgui.sf/vstgui/vstguidebug.o: vstgui.sf/vstgui/vstguidebug.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui.sf/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui.sf/vstgui/vstguidebug.o vstgui.sf/vstgui/vstguidebug.cpp

${OBJECTDIR}/vstgui.sf/vstgui/winfileselector.o: vstgui.sf/vstgui/winfileselector.cpp 
	${MKDIR} -p ${OBJECTDIR}/vstgui.sf/vstgui
	${RM} $@.d
	$(COMPILE.cc) -g -DDEVELOPMENT=1 -DVSTGUI_ENABLE_DEPRECATED_METHODS=1 -D_DEBUG -I. -Ipublic.sdk/source/vst -Ivstgui.sf -fpermissive -MMD -MP -MF $@.d -o ${OBJECTDIR}/vstgui.sf/vstgui/winfileselector.o vstgui.sf/vstgui/winfileselector.cpp

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
