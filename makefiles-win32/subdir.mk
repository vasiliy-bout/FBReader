!include $(ROOTDIR)\makefiles-win32\config.mk

.SUFFIXES:


!ifndef OBJECTS

all: *.cpp
	@$(MAKE) /f "$(ROOTDIR)\makefiles-win32\subdir.mk" SOURCES="$?" OBJECTS="$(?:.cpp=.obj)" all


!elseif [ cd "$(OUTDIR)" && dir /B $(OBJECTS) >nul 2>&1 ]

all:
	@$(CC) $(CFLAGS) /I "$(INCLUDE)" $(ZINCLUDE) $(SOURCES)


!else

.SUFFIXES: .cpp .obj .h

all: $(OBJECTS)

$(OBJECTS) : "$(OUTDIR)\$(@F)"

.cpp{$(OUTDIR)}.obj:
	@$(CC) $(CFLAGS) /I "$(INCLUDE)" $(ZINCLUDE) "$<"

.cpp.obj:


!endif


clean:
