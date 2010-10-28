@echo off

SET INCLUDE_DIRS=zlibrary\core\include zlibrary\text\include

FOR %%D IN (%INCLUDE_DIRS%) DO (
	call :build_links %%D
)

pause
exit /B 0


:build_links

SET INCLUDEDIR=%1

echo Processing %INCLUDEDIR%

FOR /R %INCLUDEDIR% %%I IN (*.h) DO (
	DIR /B /A:L %%I >nul 2>&1
	IF ERRORLEVEL 1 (
		call :build_link %%I
	)
)

exit /B 0


:build_link

SET file=%1
SET name=%~nx1
SET linkname=%~nx1.link

ren %file% %linkname%
FOR /F "usebackq tokens=1*" %%J IN ("%~dp1%linkname%") DO (
	IF "%%J"=="link" (
rem		echo Make link: %file% --^> "%~dp1%%K"
		mklink %file% "%~dp1%%K"
		del %~dp1%linkname%
	) else IF "%%K"=="" (
rem		echo Make link: %file% --^> "%~dp1%%J"
		mklink %file% "%~dp1%%J"
		del %~dp1%linkname%
	) else (
		ren %~dp1%linkname% %name%
		echo Unable to create link from %file%
	)
)
exit /B 0
