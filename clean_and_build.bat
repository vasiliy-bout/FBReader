@echo off

IF NOT "%VCVARS_SET%"=="TRUE" (
	call vcvars.bat
)

IF NOT "%VCVARS_SET%"=="TRUE" (
	echo Unable to setup environment
	exit /B 0
)


nmake /f Makefile.win32 clean

nmake /f Makefile.win32

pause
