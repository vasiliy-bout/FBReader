@echo off

IF "%~2"=="" (
	echo Usage: %0 ^<platform^> ^<install_dir^>
	exit /B 1
)


FOR %%I IN (desktop openzaurus_640x480 pdaxrom pepperpad3 zaurus_640x480 macosx maemo opensimpad-0.9.0 qvfb openzaurus_240x320 pma400 zaurus_240x320 moto win32) DO (
	IF "%~1"=="%%I" goto 1_%~1
)

echo %0: Unknown platform "%~1"
exit /B 1

:1_desktop
:1_openzaurus_640x480
:1_pdaxrom
:1_pepperpad3
:1_zaurus_640x480
:1_macosx
	SET platform=desktop
	goto end_1

:1_maemo
	SET platform=maemo
	goto end_1

:1_opensimpad-0.9.0
:1_qvfb
	SET platform=640x480
	goto end_1

:1_openzaurus_240x320
:1_pma400
:1_zaurus_240x320
:1_moto
	SET platform=240x320
	goto end_1

:1_win32
	SET platform=win32
	goto end_1

:end_1


SET installdir=%~2

FOR %%I IN (data\help\MiniHelp.%platform%.*.fb2) DO (
	FOR /F "tokens=3 delims=." %%J IN ("%%~nI") DO (
		copy /Y "%%I" "%installdir%\Minihelp.%%J.fb2" || exit /B 1
	)
)
