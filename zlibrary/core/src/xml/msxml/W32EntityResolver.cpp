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

#include <vector>
#include <string>

#include "W32EntityResolver.h"

#include <ZLXMLReader.h>


W32EntityResolver::W32EntityResolver(ZLXMLReader &reader) : myReader(reader), myRefs(1) {
}

W32EntityResolver::~W32EntityResolver() {
}


HRESULT STDMETHODCALLTYPE W32EntityResolver::resolveEntity(const wchar_t *pwchPublicId,
		const wchar_t *pwchSystemId, VARIANT *pvarInput) {

	/*const std::vector<std::string> &dtds = myReader.externalDTDs();
	for (std::vector<std::string>::const_iterator it = dtds.begin(); it != dtds.end(); ++it) {
		ZLFile dtdFile(*it);
		shared_ptr<ZLInputStream> entityStream = dtdFile.inputStream();
		if (!entityStream.isNull() && entityStream->open()) {
			const size_t BUFSIZE = 2048;
			char buffer[BUFSIZE];
			size_t length;
			do {
				length = entityStream->read(buffer, BUFSIZE);
				if (XML_Parse(entityParser, buffer, length, 0) == XML_STATUS_ERROR) {
					break;
				}
			} while (length == BUFSIZE);
		}
	}*/

	return S_OK;
}



HRESULT STDMETHODCALLTYPE W32EntityResolver::QueryInterface(REFIID riid, void **ppvObject) {
	if (ppvObject == 0) {
		return E_POINTER;
	}
	if (riid == __uuidof(IUnknown) || riid == __uuidof(ISAXEntityResolver)) {
		*ppvObject = this;
		AddRef();
		return S_OK;
	}
	*ppvObject = 0;
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE W32EntityResolver::AddRef() {
	return (ULONG)InterlockedIncrement(&myRefs);
}

ULONG STDMETHODCALLTYPE W32EntityResolver::Release() {
	ULONG res = (ULONG) InterlockedDecrement(&myRefs);
	if (res == 0) {
		delete this;
	}
	return res;
}
