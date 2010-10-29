!if "$(TARGET_ARCH)$(UI_TYPE)" == ""
!include $(ROOTDIR)\makefiles-win32\target.mk
!endif

!if "$(TARGET_ARCH)" == ""
!error TARGET_ARCH is not defined. Please edit $(ROOTDIR)\makefiles-win32\target.mk
!endif

!if "$(UI_TYPE)" == ""
!error UI_TYPE is not defined. Please edit $(ROOTDIR)\makefiles-win32\target.mk
!endif

!if "$(TARGET_ARCH)" != "win32"
!error TARGET_ARCH must be win32. Please edit $(ROOTDIR)\makefiles-win32\target.mk
!endif

!if "$(UI_TYPE)" != "win32"
!error UI_TYPE must be win32. Please edit $(ROOTDIR)\makefiles-win32\target.mk
!endif

!if "$(TARGET_STATUS)" == ""
TARGET_STATUS = release
!endif


!if "$(TARGET_STATUS)" == "release"
OUTFOLDER = Release
!elseif "$(TARGET_STATUS)" == "debug"
OUTFOLDER = Debug
!elseif "$(TARGET_STATUS)" == "profile"
OUTFOLDER = Profile
!else
!error Unknown TARGET_STATUS. Can be "release", "debug" or "profile". Please edit $(ROOTDIR)\makefiles-win32\target.mk
!endif
