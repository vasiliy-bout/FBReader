@echo off

call vcvars.bat

pushd "deps"
call clean.bat
call makelibs.bat
popd

nmake /nologo /f Makefile.win32 includes

pause
