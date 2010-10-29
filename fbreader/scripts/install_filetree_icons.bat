@echo off

IF "%~2"=="" (
	echo Usage: %0 ^<platform^> ^<install_dir^>
	exit /B 1
)


FOR %%I IN (desktop macosx maemo openzaurus_240x320 openzaurus_640x480 pdaxrom pepperpad3 pma400 qvfb win32 zaurus_240x320 zaurus_640x480 opensimpad-0.9.0 moto) DO (
	IF "%~1"=="%%I" goto 1_%~1
)

echo %0: Unknown platform "%~1"
exit /B 1


:1_desktop
:1_macosx
:1_moto
:1_pepperpad3
:1_qvfb
:1_opensimpad-0.9.0
	SET images=22x22-transparent-png
	goto end_1

:1_maemo
:1_openzaurus_640x480
:1_pdaxrom
:1_zaurus_640x480
	SET images=32x32-transparent-png
	goto end_1

:1_openzaurus_240x320
:1_pma400
:1_zaurus_240x320
	SET images=12x12-png
	goto end_1

:1_win32
	SET images=22x22-transparent-ico
	goto end_1

:end_1


SET installdir=%~2

IF NOT "%images%"=="" (
	copy /Y "data\icons\filetree\%images%\*.*" "%installdir%" || exit /B 1
)
