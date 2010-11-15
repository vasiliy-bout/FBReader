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

#include "ZLMSXMLReaderInternal.h"

#include <ZLInputStream.h>

#include "../ZLXMLReader.h"

#include "W32ContentHandler.h"
#include "W32ErrorHandler.h"
#include "W32SynchronousStream.h"
#include "W32EntityResolver.h"


ZLMSXMLReaderInternal::ZLMSXMLReaderInternal(ZLXMLReader &reader, const char *encoding) :
		myReader(reader), mySaxReader(0), myContentHandler(0), myErrorHandler(0), myEntityResolver(0) {

	HRESULT res = CoInitializeEx(0, COINIT_MULTITHREADED | COINIT_SPEED_OVER_MEMORY);
	if (res != S_OK && res != S_FALSE && res != RPC_E_CHANGED_MODE) {
		return;
	}

	CoCreateInstance(CLSID_SAXXMLReader60, 0, CLSCTX_INPROC_SERVER, IID_ISAXXMLReader, (LPVOID*)&mySaxReader);
	if (mySaxReader == 0) {
		return;
	}

	const VARIANT_BOOL parseDtd = myReader.externalDTDs().empty() ? VARIANT_FALSE : VARIANT_TRUE;
	if (FAILED(mySaxReader->putFeature(L"http://xml.org/sax/features/namespace-prefixes", VARIANT_TRUE))
			|| FAILED(mySaxReader->putFeature(L"prohibit-dtd", VARIANT_FALSE))
			|| FAILED(mySaxReader->putFeature(L"http://xml.org/sax/features/external-general-entities", parseDtd))
			|| FAILED(mySaxReader->putFeature(L"http://xml.org/sax/features/external-parameter-entities", parseDtd))) {
		mySaxReader->Release();
		mySaxReader = 0;
		return;
	}

	if (encoding != 0 && FAILED(setEncoding(encoding))) {
		mySaxReader->Release();
		mySaxReader = 0;
		return;
	}

	myContentHandler = new W32ContentHandler(myReader);
	myErrorHandler = new W32ErrorHandler();
	myEntityResolver = new W32EntityResolver(myReader);
	mySaxReader->putContentHandler(myContentHandler);
	mySaxReader->putErrorHandler(myErrorHandler);
	mySaxReader->putEntityResolver(myEntityResolver);
}

ZLMSXMLReaderInternal::~ZLMSXMLReaderInternal() {
	if (mySaxReader != 0) {
		mySaxReader->Release();
	}
	if (myContentHandler != 0) {
		myContentHandler->Release();
	}
	if (myErrorHandler != 0) {
		myErrorHandler->Release();
	}
	if (myEntityResolver != 0) {
		myEntityResolver->Release();
	}
}


HRESULT ZLMSXMLReaderInternal::setEncoding(const char *encoding) {
	if (*encoding == '\0') {
		return S_OK;
	}
	int length = MultiByteToWideChar(CP_UTF8, 0, encoding, -1, 0, 0);
	if (length <= 0) {
		return  E_FAIL;
	}

	wchar_t *buffer = new wchar_t[length];
	length = MultiByteToWideChar(CP_UTF8, 0, encoding, -1, buffer, length);
	if (length == 0) {
		return E_FAIL;
	}

	VARIANT charset;
	VariantInit(&charset);
	V_VT(&charset) = VT_BSTR;
	V_BSTR(&charset) = SysAllocString(buffer);
	delete[] buffer;

	return mySaxReader->putProperty(L"charset", charset);
}

void ZLMSXMLReaderInternal::init(const char *encoding) {
	if (mySaxReader == 0) {
		return;
	}
	if (encoding == 0) {
		return;
	}
	if (FAILED(setEncoding(encoding))) {
		mySaxReader->Release();
		mySaxReader = 0;
	}
}


bool ZLMSXMLReaderInternal::readDocument(shared_ptr<ZLInputStream> stream) {
	if (mySaxReader == 0) {
		return false;
	}

	VARIANT input;
	VariantInit(&input);
	V_VT(&input) = VT_UNKNOWN;
	V_UNKNOWN(&input) = new W32SynchronousStream(stream);

	HRESULT res = mySaxReader->parse(input);

	VariantClear(&input);

	return SUCCEEDED(res);
}
