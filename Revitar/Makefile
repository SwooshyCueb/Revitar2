## -*- Makefile -*-
##
## Copyright (C) 2015 Markus Kitsinger (SwooshyCueb) <root@swooshalicio.us>
##
## This Source Code Form is subject to the terms of the Mozilla Public
## License, v. 2.0. If a copy of the MPL was not distributed with this
## file, You can obtain one at http://mozilla.org/MPL/2.0/.
##

CXX=g++
WINDRES=windres
MKDIR=mkdir
CP=cp
GREP=grep
SED=sed
LD=ld
RM=rm

CXXFLAGS= -O2
REVITAR_CXXFLAGS= -fpermissive
VSTSDK_CXXFLAGS= -Wno-write-strings
VSTGUI_CXXFLAGS= -Wno-write-strings

INCLUDEDIRS= \
	"$(VSTSDK)\public.sdk\source\vst2.x" \
	"$(VSTSDK)"

LD_LIBDIRS=
LD_LIBS= \
	gdi32 \
	gdiplus \
	ole32 \
	uuid
LDFLAGS= -static-libstdc++ -static-libgcc -shared

TRIPLET=$(shell $(CXX) -v 2>&1 | $(GREP) Target | $(SED) 's/.* //')

OBJDIR=obj/$(TRIPLET)
BUILDDIR=build/$(TRIPLET)
VSTSDK=vst3sdk
VSTOBJDIR=$(OBJDIR)/VSTSDK

# By default, build using the VST3 SDK
all: $(BUILDDIR)/Revitar2.dll

print:
	@echo CXX = $(CXX)
	@echo CXXFLAGS = $(CXXFLAGS)

CXXOBJS = \
	$(OBJDIR)/RevEditor.o \
	$(OBJDIR)/Revitar.o \
	$(OBJDIR)/controls.o \
	$(OBJDIR)/presets.o

VST2OBJS= \
	$(VSTOBJDIR)/audioeffect.o \
	$(VSTOBJDIR)/audioeffectx.o \
	$(VSTOBJDIR)/vstplugmain.o

VSTGUIOBJS= \
	$(VSTOBJDIR)/aeffguieditor.o \
	$(VSTOBJDIR)/vstcontrols.o \
	$(VSTOBJDIR)/vstgui.o

RESOBJS= \
	$(OBJDIR)/resources.o

OBJS= \
	$(CXXOBJS) \
	$(VST2OBJS) \
	$(VSTGUIOBJS) \
	$(RESOBJS)

$(BUILDDIR)/Revitar2.dll: $(OBJS) $(BUILDDIR)
	$(CXX) -o $@ $(OBJS) $(foreach d, $(LD_LIBDIRS), -L$d) $(foreach l, $(LD_LIBS), -l$l) $(LDFLAGS)

$(CXXOBJS): $(OBJDIR)/%.o: %.cpp $(OBJDIR)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(REVITAR_CXXFLAGS) $(foreach d, $(INCLUDEDIRS), -I$d)

$(OBJDIR)/resources.o: Revitar.rc $(OBJDIR)
	$(WINDRES) -o $@ $< $(foreach d, $(INCLUDEDIRS), -I$d)

$(VST2OBJS): $(VSTOBJDIR)/%.o: $(VSTSDK)/public.sdk/source/vst2.x/%.cpp $(VSTOBJDIR)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(VSTSDK_CXXFLAGS) $(foreach d, $(INCLUDEDIRS), -I$d)

$(VSTGUIOBJS): $(VSTOBJDIR)/%.o: $(VSTSDK)/vstgui.sf/vstgui/%.cpp $(VSTOBJDIR)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(VSTGUI_CXXFLAGS) $(foreach d, $(INCLUDEDIRS), -I$d)

$(OBJDIR):
	$(MKDIR) -p $@

$(VSTOBJDIR):
	$(MKDIR) -p $@

$(BUILDDIR):
	$(MKDIR) -p $@

clean:
	$(RM) -rf $(BUILDDIR)
	$(RM) -rf $(OBJDIR)

