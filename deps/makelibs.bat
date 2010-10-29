@echo off

SETLOCAL ENABLEDELAYEDEXPANSION

IF NOT "%VCVARS_SET%"=="TRUE" (
	call ..\vcvars.bat
)

IF NOT "%VCVARS_SET%"=="TRUE" (
	echo Unable to setup environment
	exit /B 0
)


mkdir "lib"

del /Q /S  "*.def" > nul 2>&1
del /Q /S  "*.def.in" > nul 2>&1
del /Q /S  "lib\*.exp" > nul 2>&1
del /Q /S  "lib\*.lib" > nul 2>&1


FOR %%F IN ("..\libs\win32\*.dll") DO (
	call :processdll "%%F"
)


FOR %%F IN ("*.def") DO (
	call :processdef "%%F"
)

del /Q /S  "*.def" > nul 2>&1
del /Q /S  "*.def.in" > nul 2>&1

copy /Y "lib-src\*.lib" "lib\"

exit /B 0



:processdll
	SET file=%~1
	SET filename=%~nx1
	SET deffile=%filename:.dll=.def%

	echo Making "%deffile%"

	dumpbin /EXPORTS "%file%" > "%deffile%.in"
	call :processin "%deffile%.in" "%deffile%"
exit /B 0


:processin
	SET ABC=0
	echo EXPORTS> %2
	FOR /F "usebackq tokens=1-3*" %%I IN ("%~1") DO (
		IF "%%I_%%J_%%K_%%L"=="Summary___" goto processdef_break
		IF "!ABC!"=="1" (
			echo %%L>> %2
		)
		IF "%%I_%%J_%%K_%%L"=="ordinal_hint_RVA_name" SET ABC=1
	)
:processdef_break
exit /B 0


:processdef
	SET file=%~1
	SET filename=%~nx1
	lib /NOLOGO /MACHINE:X86 /DEF:"%file%" /out:"lib\%filename:.def=.lib%"
exit /B 0
