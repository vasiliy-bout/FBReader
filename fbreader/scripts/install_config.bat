@echo off

IF "%~3"=="" (
	echo Usage: %0 ^<platform^> ^<ui_type^> ^<install_dir^>
	exit /B 1
)


FOR %%I IN (desktop win32 macosx maemo pepperpad3 zaurus_640x480) DO (
	IF "%~1"=="%%I" goto 1_%~1
)

goto end_1

:1_desktop
:1_win32
	goto end_1

:1_macosx
	goto end_1

:1_maemo
	IF "%~2"=="maemo5" (
		SET config_file=data/default/config.maemo5.xml
	) ELSE (
		SET config_file=data/default/config.maemo.xml
	)
	goto end_1

:1_pepperpad3
	goto end_1

:1_zaurus_640x480
	goto end_1

:end_1


SET installdir=%~3

IF NOT "%config_file%"=="" (
	copy /Y "%config_file%" "%installdir%\config.xml" || exit /B 1
)
