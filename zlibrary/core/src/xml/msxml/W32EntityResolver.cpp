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
#include <ZLFile.h>
#include <ZLInputStream.h>
#include <ZLibrary.h>

#include "MSXMLUtil.h"
#include "W32SynchronousStream.h"


W32EntityResolver::W32EntityResolver(ZLXMLReader &reader) : myReader(reader), myRefs(1) {
}

W32EntityResolver::~W32EntityResolver() {
}

static const std::wstring FBREADER_PUBLIC_ID = L"-//fbreader.org//formats";
static const std::wstring FBREADER_SYSTEM_PREFIX = L"http://data.fbreader.org/";

HRESULT STDMETHODCALLTYPE W32EntityResolver::resolveEntity(const wchar_t *pwchPublicId,
		const wchar_t *pwchSystemId, VARIANT *pvarInput) {

	if (FBREADER_PUBLIC_ID.compare(pwchPublicId) != 0) {
		return S_OK;
	}

	if (FBREADER_SYSTEM_PREFIX.compare(0, FBREADER_SYSTEM_PREFIX.length(),
			pwchSystemId, FBREADER_SYSTEM_PREFIX.length()) != 0) {
		return S_OK;
	}

	char *buffer = 0;
	unsigned size = 0;
	int len = MSXMLUtil::decodeWideChars(pwchSystemId + FBREADER_SYSTEM_PREFIX.length(), -1, &buffer, &size);
	if (len < 0) {
		if (buffer != 0) {
			delete[] buffer;
		}
		return E_FAIL;
	}
	std::string pattern(buffer, len);
	delete[] buffer;
	buffer = 0;

	if (ZLibrary::FileNameDelimiter.compare("/") != 0) {
		size_t index = 0;
		while ((index = pattern.find("/", index)) != -1) {
			pattern.replace(index, 1, ZLibrary::FileNameDelimiter);
		}
	}

	const std::vector<std::string> &dtds = myReader.externalDTDs();
	for (std::vector<std::string>::const_iterator it = dtds.begin(); it != dtds.end(); ++it) {
		const std::string &fileName = *it;
		if (fileName.find(pattern, 0) == -1) {
			continue;
		}
		ZLFile dtdFile(fileName);
		shared_ptr<ZLInputStream> entityStream = dtdFile.inputStream();
		if (!entityStream.isNull() && entityStream->open()) {
			VariantInit(pvarInput);
			V_VT(pvarInput) = VT_UNKNOWN;
			V_UNKNOWN(pvarInput) = new W32SynchronousStream(entityStream, true);
			return S_OK;
		}
	}
	return E_FAIL;
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
