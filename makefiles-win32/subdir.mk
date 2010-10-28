!include $(ROOTDIR)\makefiles-win32\config.mk

.SUFFIXES:
.SUFFIXES: .cpp .obj .h


!ifdef OBJECTS

all: $(OBJECTS)

$(OBJECTS) : $(OUTDIR)\$(@F)

!else

all: *.cpp
	@$(MAKE) /f "$(ROOTDIR)\makefiles-win32\subdir.mk" OBJECTS="$(?:.cpp=.obj)" all

!endif


.cpp.obj:


.cpp{$(OUTDIR)}.obj:
#	@echo Compiling $@ ...
	@$(CC) $(CFLAGS) /I "$(INCLUDE)" $(ZINCLUDE) "$<"
#	@echo OK

clean:
#	@$(RM) *.obj *.pdb
