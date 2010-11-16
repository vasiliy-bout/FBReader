/*
 * Copyright (C) 2004-2010 Geometer Plus <contact@geometerplus.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifdef WIN32
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#	include <tchar.h>
#	include <fcntl.h>
#	include <io.h>
#	include <iostream>
#endif

#include <ZLibrary.h>

#include "FBReader.h"


#ifdef WIN32

#ifdef _USE_CONSOLE
void __RedirectIOToConsole();
#	define RedirectIOToConsole()	__RedirectIOToConsole()
#else
#	define RedirectIOToConsole()	do {} while (false)
#endif

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	int argc = 1;
	char **argv = new char*[2];
	argv[0] = "FBReader";
	argv[1] = 0;

	RedirectIOToConsole();

#else
int main(int argc, char **argv) {
#endif

	if (!ZLibrary::init(argc, argv)) {
		return 1;
	}
	ZLibrary::run(new FBReader(argc == 1 ? std::string() : argv[1]));
	ZLibrary::shutdown();
	return 0;
}

#ifdef WIN32
#ifdef _USE_CONSOLE

#ifndef _MAX_CONSOLE_LINES
#define _MAX_CONSOLE_LINES 500;
#endif
static const WORD MAX_CONSOLE_LINES = _MAX_CONSOLE_LINES;

void __redirectStream(DWORD nStdHanle, FILE *stream, const char *mode) {
	intptr_t lStdHandle = (intptr_t) GetStdHandle(nStdHanle);
	int hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	FILE *fp = _fdopen(hConHandle, mode);
	*stream = *fp;
	setvbuf(stream, NULL, _IONBF, 0);
}

void __RedirectIOToConsole() {
	CONSOLE_SCREEN_BUFFER_INFO coninfo;

	AllocConsole();
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = MAX_CONSOLE_LINES;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

	__redirectStream(STD_OUTPUT_HANDLE, stdout, "w");
	__redirectStream(STD_INPUT_HANDLE, stdin, "r");
	__redirectStream(STD_ERROR_HANDLE, stderr, "w");

	std::ios::sync_with_stdio(true);
}

#endif
#endif
