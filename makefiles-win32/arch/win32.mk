SHAREDIR = \share
IMAGEDIR = $(SHAREDIR)\icons
APPIMAGEDIR = $(IMAGEDIR)

SHAREDIR_MACRO = ~~\\\\share
IMAGEDIR_MACRO = $(SHAREDIR_MACRO)\\\\icons
APPIMAGEDIR_MACRO = $(IMAGEDIR_MACRO)

ZLSHARED = no

CC = cl /nologo
AR = lib /nologo
LD = link /nologo
RC = rc

CFLAGS = $(CFLAGS) /D "WIN32" /D "_WIN32=1" /D "_WINDOWS" /D "_CRT_SECURE_NO_WARNINGS" /D "_UNICODE" /D "UNICODE" /W3 /c /TP /errorReport:none /D "_WIN32_IE=0x0501" /D "_WIN32_WINNT=0x0501" /D "WINVER=0x0500" /D "XMLCONFIGHOMEDIR=\"~\\..\""
LDFLAGS = $(LDFLAGS) /SUBSYSTEM:WINDOWS /DYNAMICBASE /NXCOMPAT /MACHINE:X86 /errorReport:none


#CFLAGS = $(CFLAGS) /D "_USE_CONSOLE"
#CFLAGS = $(CFLAGS) /D "_MAX_CONSOLE_LINES=500"


UILIBS = gdi32.lib comctl32.lib comdlg32.lib libpng-3.lib jpeg62.lib libungif4.lib libtiff3.lib

RM = DEL /Q
RM_QUIET = DEL /Q

BUILD_RESOURCE_OBJECT = yes
INCREMENTAL_LINK = yes

.resources: .outdir
	@echo Creating resource object...
	@echo ApplicationIcon ICON "data/icons/application/win32.ico" > "$(OUTFOLDER)\$(TARGET).rc"
	@echo 1 24 "win32/$(TARGET_STATUS).manifest" >> "$(OUTFOLDER)\$(TARGET).rc"
	@$(RC) /fo "$(OUTFOLDER)\$(TARGET).res" "$(OUTFOLDER)\$(TARGET).rc"
	@$(RM_QUIET) "$(OUTFOLDER)\$(TARGET).rc"
	@echo OK
