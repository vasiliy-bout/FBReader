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

#include <iostream>

#include <ZLStringUtil.h>

#include "W32ErrorHandler.h"


W32ErrorHandler::W32ErrorHandler() : myRefs(1) {
}

W32ErrorHandler::~W32ErrorHandler() {
}


static std::string pwch2str(const wchar_t *pwchStr, int cchStr) {
	if (pwchStr == 0) {
		return "(null)";
	}
	char buffer[1024];
	int len;
	len = WideCharToMultiByte(CP_UTF8, 0, pwchStr, cchStr, buffer, 1024, 0, 0);
	std::string str(buffer, len);
	ZLStringUtil::stripWhiteSpaces(str);
	return str;
}

template <class _Elem>
		inline std::basic_ostream<_Elem> &operator << (std::basic_ostream<_Elem> &os, std::string &str) {
	return os << str.c_str();
}


HRESULT STDMETHODCALLTYPE W32ErrorHandler::error(ISAXLocator *pLocator, const wchar_t *pwchErrorMessage, HRESULT hrErrorCode) {
	int line = -1, col = -1;
	const wchar_t *sysId;
	if (pLocator != 0) {
		pLocator->getLineNumber(&line);
		pLocator->getColumnNumber(&col);
		pLocator->getSystemId(&sysId);
	}
	std::cerr << "SAX error (" << hrErrorCode << "): \"" << pwch2str(pwchErrorMessage, -1) << "\""
			<< " at " << pwch2str(sysId, -1) << " (" << line << ", " << col << ")" << std::endl;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE W32ErrorHandler::fatalError(ISAXLocator *pLocator, const wchar_t *pwchErrorMessage, HRESULT hrErrorCode) {
	int line = -1, col = -1;
	const wchar_t *sysId;
	if (pLocator != 0) {
		pLocator->getLineNumber(&line);
		pLocator->getColumnNumber(&col);
		pLocator->getSystemId(&sysId);
	}
	std::cerr << "SAX fatal error (" << hrErrorCode << "): \"" << pwch2str(pwchErrorMessage, -1) << "\""
			<< " at " << pwch2str(sysId, -1) << " (" << line << ", " << col << ")" << std::endl;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE W32ErrorHandler::ignorableWarning(ISAXLocator *pLocator, const wchar_t *pwchErrorMessage, HRESULT hrErrorCode) {
	int line = -1, col = -1;
	const wchar_t *sysId;
	if (pLocator != 0) {
		pLocator->getLineNumber(&line);
		pLocator->getColumnNumber(&col);
		pLocator->getSystemId(&sysId);
	}
	std::cerr << "SAX warning (" << hrErrorCode << "): \"" << pwch2str(pwchErrorMessage, -1) << "\""
			<< " at " << pwch2str(sysId, -1) << " (" << line << ", " << col << ")" << std::endl;
	return S_OK;
}




HRESULT STDMETHODCALLTYPE W32ErrorHandler::QueryInterface(REFIID riid, void **ppvObject) {
	if (ppvObject == 0) {
		return E_POINTER;
	}
	if (riid == __uuidof(IUnknown) || riid == __uuidof(ISAXErrorHandler)) {
		*ppvObject = this;
		AddRef();
		return S_OK;
	}
	*ppvObject = 0;
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE W32ErrorHandler::AddRef() {
	return (ULONG)InterlockedIncrement(&myRefs);
}

ULONG STDMETHODCALLTYPE W32ErrorHandler::Release() {
	ULONG res = (ULONG) InterlockedDecrement(&myRefs);
	if (res == 0) {
		delete this;
	}
	return res;
}
