
CURDIR = $(MAKEDIR)

!ifndef VERSION
!error VERSION hasn't been specified
!endif

!ifndef ARCHITECTURE
!error ARCHITECTURE hasn't been specified
!endif

!ifndef DIST_DIR
!error DIST_DIR hasn't been specified
!endif


CURDIR = $(MAKEDIR)
TMPDIR = $(CURDIR)\fbreader-$(VERSION)


motopkg tarball debian ipk debipk:
	@echo Unable to build this Architecture


nsi:
	@echo Building $(ARCHITECTURE) nsi package...
	@$(MAKE) /f "$(DIST_DIR)\nsi\$(ARCHITECTURE)\rules.win32" build 1> $(ARCHITECTURE)-nsi.log 2>&1
	@-mkdir $(TMPDIR)
	@$(MAKE) /f "$(DIST_DIR)\nsi\$(ARCHITECTURE)\rules.win32" "DESTDIR=$(TMPDIR)" install 1>> $(ARCHITECTURE)-nsi.log 2>&1
	@sed "s#@VERSION@#$(VERSION)#" "$(DIST_DIR)\nsi\$(ARCHITECTURE)\control.nsi" > "$(TMPDIR)\control.nsi"
	@cd $(TMPDIR) & makensis control.nsi 1>> "$(CURDIR)\$(ARCHITECTURE)-nsi.log" 2>&1
	@move "$(TMPDIR)\*.exe" .
	@$(MAKE) /f "$(DIST_DIR)\nsi\$(ARCHITECTURE)\rules.win32" clean 1> nul 2>&1
	@rmdir /S /Q "$(TMPDIR)"
	@del "$(CURDIR)\$(ARCHITECTURE)-nsi.log"
	@echo OK
