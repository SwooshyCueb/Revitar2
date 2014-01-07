lib_LTLIBRARIES = libvstgui.la


libvstgui_la_SOURCES = vstgui/aeffguieditor.cpp vstgui/cdatabrowser.cpp vstgui/cfileselector.cpp vstgui/cscrollview.cpp \
vstgui/ctabview.cpp vstgui/ctooltipsupport.cpp vstgui/cvstguitimer.cpp vstgui/vstcontrols.cpp vstgui/vstgui.cpp \
vstgui/vstguidebug.cpp vstgui/winfileselector.cpp vstgui/plugguieditor.cpp 


include_HEADERS = vstgui/aeffguieditor.h vstgui/cdatabrowser.h vstgui/cfileselector.h vstgui/cscrollview.h vstgui/ctabview.h \
vstgui/ctooltipsupport.h vstgui/cvstguitimer.h vstgui/vstcontrols.h vstgui/vstgui.h vstgui/vstguidebug.h vstgui/vstkeycode.h \
vstgui/plugguieditor.h 


if COCOASUPPORT
    libvstgui_la_SOURCES += vstgui/cocoasupport.mm 
    include_HEADERS += vstgui/cocoasupport.h 
endif

install-exec-hook:
	cd $(DESTDIR)$(libdir) && \
	mv -f libvstgui.la libvstgui-$(VERSION).la && \
	$(LN_S) libvstgui-$(VERSION).la libvstgui.la