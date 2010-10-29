@echo off

IF "%~3"=="" (
	echo Usage: %0 ^<platform^> ^<ui_type^> ^<install_dir^>
	exit /B 1
)


FOR %%I IN (desktop win32 macosx maemo pepperpad3 zaurus_640x480) DO (
	IF "%~1"=="%%I" goto 1_%~1
)

SET toolbar_file=data\default\toolbar.short.xml
goto end_1

:1_desktop
:1_win32
	SET toolbar_file=data\default\toolbar.desktop.xml
	SET fullscreen_toolbar_file=data\default\fullscreen_toolbar.desktop.xml
	goto end_1

:1_macosx
	SET toolbar_file=data\default\toolbar.macosx.xml
	goto end_1

:1_maemo
	IF "%~2"=="maemo5" (
	) ELSE (
		SET toolbar_file=data\default\toolbar.maemo.xml
	)
	goto end_1

:1_pepperpad3
	SET toolbar_file=data\default\toolbar.full.xml
	goto end_1

:1_zaurus_640x480
	SET toolbar_file=data\default\toolbar.zaurus.xml
	goto end_1

:end_1



FOR %%I IN (zaurus_640x480 zaurus_240x320 pma400 maemo moto) DO (
	IF "%~1"=="%%I" goto 2_%~1
)

goto end_2

:2_zaurus_640x480
:2_zaurus_240x320
:2_pma400
	SET menubar_file=data\default\menubar.xml
	goto end_2

:2_maemo
	IF "%~2"=="maemo5" (
		SET menubar_file=data\default\menubar.maemo5.xml
	) ELSE (
		SET menubar_file=data\default\menubar.xml
	)
	goto end_2

:2_moto
	SET menubar_file=data\default\menubar.moto.xml
	goto end_2

:end_2


SET installdir=%~3

IF NOT "%fullscreen_toolbar_file%"=="" (
	copy /Y "%fullscreen_toolbar_file%" "%installdir%\fullscreen_toolbar.xml" || exit /B 1
)

IF NOT "%toolbar_file%"=="" (
	copy /Y "%toolbar_file%" "%installdir%\toolbar.xml" || exit /B 1
)

IF NOT "%menubar_file%"=="" (
	copy /Y "%menubar_file%" "%installdir%\menubar.xml" || exit /B 1
)
