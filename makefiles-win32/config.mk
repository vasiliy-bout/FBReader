# before include this makefile define this macros:
# ROOTDIR
# TARGET
# PROJECTDIR

!include $(ROOTDIR)\makefiles-win32\platforms.mk
!include $(ROOTDIR)\makefiles-win32\version.mk

MAKE = nmake /nologo ROOTDIR=$(ROOTDIR)
LIBMAKE = $(MAKE) ZLSHARED=$(ZLSHARED)

!include $(ROOTDIR)\makefiles-win32\arch\$(TARGET_ARCH).mk

!ifndef BINDIR
BINDIR = $(INSTALLDIR)\bin
!endif
!ifndef LIBDIR
LIBDIR = $(INSTALLDIR)\lib
!endif
!ifndef INCDIR
INCDIR = $(INSTALLDIR)\include
!endif
!ifndef SHAREDIR
SHAREDIR = $(INSTALLDIR)\share
!endif

!ifndef INSTALLDIR_MACRO
INSTALLDIR_MACRO = $(INSTALLDIR)
!endif
!ifndef LIBDIR_MACRO
LIBDIR_MACRO = $(LIBDIR)
!endif
!ifndef SHAREDIR_MACRO
SHAREDIR_MACRO = $(SHAREDIR)
!endif
!ifndef IMAGEDIR_MACRO
IMAGEDIR_MACRO = $(IMAGEDIR)
!endif
!ifndef APPIMAGEDIR_MACRO
APPIMAGEDIR_MACRO = $(APPIMAGEDIR)
!endif

!ifndef XML_LIBS
XML_LIBS = libexpat-1.lib
!endif
!ifndef ARCHIVER_LIBS
ARCHIVER_LIBS = zlib.lib libbz2.lib
!endif
!ifndef NETWORK_LIBS
NETWORK_LIBS = libcurl-3.lib
!endif

# Options:
CFLAGS = $(CFLAGS) /D "BASEDIR=\"$(SHAREDIR_MACRO)\"" /D "IMAGEDIR=\"$(IMAGEDIR_MACRO)\"" /D "APPIMAGEDIR=\"$(APPIMAGEDIR_MACRO)\"" /D "VERSION=\"$(VERSION)\""


!if "$(ZLSHARED)" == "yes"
!error ZLSHARED == yes is unsupported
CFLAGS = $(CFLAGS) /D "ZLSHARED"
!endif

!if "$(TARGET_STATUS)" == "release"
CFLAGS = $(CFLAGS) /D "NDEBUG" /O2 /Oi /GL /Ot /GF /EHsc /Gy /MD /Zi
LDFLAGS = $(LDFLAGS) /INCREMENTAL:NO /OPT:REF /OPT:ICF /LTCG
ARFLAGS = $(ARFLAGS) /LTCG
!endif
!if "$(TARGET_STATUS)" == "debug"
CFLAGS = $(CFLAGS) /D "_DEBUG" /Od /Gm /EHsc /RTC1 /MDd /ZI
LDFLAGS = $(LDFLAGS) /INCREMENTAL /NODEFAULTLIB:"msvcrt" /DEBUG
!endif
!if "$(TARGET_STATUS)" == "profile"
!error Unsupported TARGET_STATUS: $(TARGET_STATUS)
!endif

CFLAGS = $(CFLAGS) /Fd"$(OUTDIR)\\" /Fo"$(OUTDIR)\\"

ZINCLUDE = /I "$(ROOTDIR)\zlibrary\core\include" /I "$(ROOTDIR)\zlibrary\text\include"


CORE_LIBS = /LIBPATH:"$(ROOTDIR)\zlibrary\core\$(OUTFOLDER)" zlcore.lib
TEXT_LIBS = /LIBPATH:"$(ROOTDIR)\zlibrary\text\$(OUTFOLDER)" zltext.lib

!if "$(ZLSHARED)" == "yes"
#CORE_LIBS = $(CORE_LIBS)
#TEXT_LIBS = $(TEXT_LIBS)
!else
CORE_LIBS = $(CORE_LIBS) /LIBPATH:"$(ROOTDIR)\zlibrary\ui\$(OUTFOLDER)" zlui-$(UI_TYPE).lib $(UILIBS) $(XML_LIBS) $(ARCHIVER_LIBS) $(NETWORK_LIBS) libsqlite3-0.lib
TEXT_LIBS = $(TEXT_LIBS) libfribidi-0.lib libidn-11.lib liblinebreak-1.lib
!endif

!if "$(BUILD_RESOURCE_OBJECT)" != "yes"
.resources:
!endif


OUTDIR = $(PROJECTDIR)\$(OUTFOLDER)

.outdir:
	@IF NOT EXIST "$(OUTDIR)" @( @mkdir "$(OUTDIR)" )


# Is this necessary???
STDLIBS = kernel32.lib user32.lib winspool.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
ZINCLUDE = $(ZINCLUDE) /I "D:\work\fbreader-vs\src\unix\include"
CORE_LIBS = $(CORE_LIBS) /LIBPATH:"D:\work\fbreader-vs\src\unix\lib" $(STDLIBS)