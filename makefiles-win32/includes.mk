!include $(ROOTDIR)\makefiles-win32\config.mk

.SUFFIXES:
.SUFFIXES: .lnk .h


all: .h2lnk .lnk2h


!ifdef LINKS_FILES

.h2lnk: $(LINKS_FILES)

.h.lnk:
	@IF NOT EXIST "$@" @( \
		type "$<" > "$@" \
	)

!else

.h2lnk: *.h
	@$(MAKE) /f "$(ROOTDIR)\makefiles-win32\includes.mk" LINKS_FILES="$(?:.h=.lnk)" .h2lnk

!endif



!ifdef HEADER_FILES

.lnk2h: $(HEADER_FILES)

.lnk.h:
	@FOR /F "usebackq tokens=*" %%J IN ("$<") DO @( \
		$(MAKE) /f "$(ROOTDIR)\makefiles-win32\includes.mk" LINK_NAME="$@" LINK_TARGET="%%J" .makelink && \
		(exit 0) || \
		(echo Unable to create link "$@" to "%%J" && exit 1) \
	)

!else

.lnk2h: *.lnk
	@$(MAKE) /f "$(ROOTDIR)\makefiles-win32\includes.mk" HEADER_FILES="$(?:.lnk=.h)" .lnk2h

!endif


.makelink:
	@IF EXIST "$(LINK_NAME)" @( \
		$(RM_QUIET) "$(LINK_NAME)" \
	)
	@mklink "$(LINK_NAME)" "$(LINK_TARGET:/=\)"
