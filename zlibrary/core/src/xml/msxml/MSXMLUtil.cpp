/*
 * Copyright (C) 2010 Geometer Plus <contact@geometerplus.com>
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

#include "MSXMLUtil.h"


static const size_t MINIMAL_SIZE = 16;

/*
 * The function returns -1 if it doesn't succeed.
 * If the function succeeds, it returns the length of the string written to the *pBuffer.
 * Null-terminator is appended to the *pBuffer.
 */
int MSXMLUtil::decodeWideChars(const wchar_t *wideChars, const int len,
		char **pBuffer, unsigned *pBufferSize) {

	if (wideChars == 0 || len == 0 || *wideChars == L'\0') {
		if (*pBuffer == 0) {
			*pBuffer = new char[*pBufferSize = MINIMAL_SIZE];
		}
		**pBuffer = '\0';
		return 0;
	}
	int decodedLength = WideCharToMultiByte(CP_UTF8, 0, wideChars, len, 0, 0, 0, 0);
	if (len > 0) {
		++decodedLength;
	}
	if (decodedLength < MINIMAL_SIZE) {
		decodedLength = MINIMAL_SIZE;
	}
	if ((unsigned) decodedLength > *pBufferSize) {
		if (*pBuffer != 0) {
			delete[] *pBuffer;
		}
		*pBuffer = new char[*pBufferSize = decodedLength];
	}
	decodedLength = WideCharToMultiByte(CP_UTF8, 0, wideChars, len, *pBuffer, *pBufferSize, 0, 0);
	if (decodedLength == 0) {
		return -1;
	}
	if (len > 0) {
		(*pBuffer)[decodedLength++] = '\0';
	}
	return decodedLength - 1;
}
