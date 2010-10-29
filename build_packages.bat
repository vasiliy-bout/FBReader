@echo off

IF NOT "%VCVARS_SET%"=="TRUE" (
	call vcvars.bat
)

IF NOT "%VCVARS_SET%"=="TRUE" (
	echo Unable to setup environment
	exit /B 0
)


for /F %%I IN (fbreader/VERSION) DO (
	SET version=%%I
)

SET tmpdir=fbreader-%version%
SET distdir=distributions
SET pkgdir=packages

IF "%1" == "" (
	echo usage:
	echo   %~nx0 ^<architecture^>
	echo .
	echo available architectures are:
	for /D %%I in (%distdir%\*) do (
		for /D %%J in (%%I\*); do (
			echo ^	%%~nJ-%%~nI
		)
	)
	pause
	exit /B 1
)

FOR /F "tokens=1* delims=-" %%I IN ("%1") DO (
	echo archtype = %%I
	echo pkgtype = %%J
	IF EXIST "%distdir%/%%J/%%I" (
		call :create_tmpdir
		call :build_package %%I %%J
		call :remove_tmpdir
	) ELSE (
		echo Unknown architecture: %%I-%%J
		exit /B 1
	)
)

echo Done.
pause
exit /B 0


:create_tmpdir
	mkdir %tmpdir%
	copy Makefile.win32 %tmpdir%
	copy build_packages.bat %tmpdir%
	xcopy /Q /I /E zlibrary %tmpdir%\zlibrary
	xcopy /Q /I /E fbreader %tmpdir%\fbreader
	xcopy /Q /I /E makefiles %tmpdir%\makefiles

	xcopy /Q /I /E deps %tmpdir%\deps
	xcopy /Q /I /E makefiles-win32 %tmpdir%\makefiles-win32

	copy README.build %tmpdir%
	copy CHANGES* %tmpdir%
	xcopy /Q /I /E distributions %tmpdir%\distributions
	copy ChangeLog %tmpdir%

	for /R %tmpdir% %%D IN (.) DO (
		IF "%%~nxD"==".svn" (
			rmdir /Q /S %%D
		)
	)

	pushd %tmpdir%
rem	nmake /nologo /f Makefile.win32 clean
	popd
exit /B 0


:remove_tmpdir
	rmdir /S /Q %tmpdir%
exit /B 0


:build_package
	SET make_package=nmake /nologo /f "makefiles-win32\packaging.mk" DIST_DIR=%distdir% VERSION=%version% ARCHITECTURE=%1 %2

	IF "%2" == "nsi" (
		pushd %tmpdir%
		%make_package%
		IF ERRORLEVEL 1 (
			echo Building failed... abort.
			popd
			pause
			exit /B 0
		)
		popd
		mkdir %pkgdir%\%1
		move /Y %tmpdir%\*.exe %pkgdir%\%1
	) else (
		echo No rule is defined for package type "$2"
	)
exit /B 0
