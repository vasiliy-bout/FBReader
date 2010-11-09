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

#include "W32ContentHandler.h"

#include "../ZLXMLReader.h"


/*
 * The function returns -1 if it doesn't succeed.
 * If the function succeeds, it returns the length of the string written to the *pBuffer.
 * Null-terminator is appended to the *pBuffer.
 */
static int decodeWideChars(const wchar_t *wideChars, const int len, char **pBuffer, unsigned *pBufferSize) {
	static const size_t MINIMAL_SIZE = 16;

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



W32ContentHandler::W32ContentHandler(ZLXMLReader &reader) : myReader(reader),
		myBuffer(0), myBufferSize(0), myRefs(1) {
}

W32ContentHandler::~W32ContentHandler() {
	if (myBuffer != 0) {
		delete[] myBuffer;
	}
	const size_t attributesSize = myAttributes.size();
	for (size_t i = 0; i < attributesSize; ++i) {
		char *ptr = myAttributes[i];
		if (ptr != 0) {
			delete[] ptr;
		}
	}
}


int W32ContentHandler::setAttributeString(int index, const wchar_t *value, int len) {
	std::vector<char*>::iterator attrIt = myAttributes.begin() + index;
	std::vector<unsigned>::iterator sizesIt = myAttributesSizes.begin() + index;
	while (attrIt != myAttributes.end() && *attrIt == 0) {
		attrIt = myAttributes.erase(attrIt);
		sizesIt = myAttributesSizes.erase(sizesIt);
	}
	if (attrIt == myAttributes.end()) {
		attrIt = myAttributes.insert(attrIt, 0);
		sizesIt = myAttributesSizes.insert(sizesIt, 0);
	}
	return decodeWideChars(value, len, &*attrIt, &*sizesIt);
}

void W32ContentHandler::terminateAttributes(int index) {
	std::vector<char*>::iterator attrIt = myAttributes.begin() + index;
	std::vector<unsigned>::iterator sizesIt = myAttributesSizes.begin() + index;
	if (attrIt == myAttributes.end() || *attrIt != 0) {
		attrIt = myAttributes.insert(attrIt, 0);
		sizesIt = myAttributesSizes.insert(sizesIt, 0);
	}
}


HRESULT W32ContentHandler::decodeAttributes(ISAXAttributes *pAttributes) {
	int attributesCount;
	HRESULT res = pAttributes->getLength(&attributesCount);
	if (res != S_OK) {
		return res;
	}

	int attr = 0;
	for (int index = 0; index < attributesCount; ++index) {
		const wchar_t *pwchQName;
		const wchar_t *pwchValue;
		int cchQName, cchValue;

		res = pAttributes->getQName(index, &pwchQName, &cchQName);
		if (res != S_OK) {
			return res;
		}
		res = pAttributes->getValue(index, &pwchValue, &cchValue);
		if (res != S_OK) {
			return res;
		}

		setAttributeString(attr++, pwchQName, cchQName);
		setAttributeString(attr++, pwchValue, cchValue);
	}
	terminateAttributes(attr);
	return S_OK;
}


HRESULT STDMETHODCALLTYPE W32ContentHandler::characters(const wchar_t *pwchChars, int cchChars) {
	if (myReader.isInterrupted()) {
		return S_OK; // TODO: interrupt???
	}
	const int len = decodeWideChars(pwchChars, cchChars, &myBuffer, &myBufferSize);
	if (len < 0) {
		return E_FAIL;
	}
	myReader.characterDataHandler(myBuffer, len);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE W32ContentHandler::startElement(const wchar_t *pwchNamespaceUri, int cchNamespaceUri,
		const wchar_t *pwchLocalName, int cchLocalName, const wchar_t *pwchQName, int cchQName,
		ISAXAttributes *pAttributes) {

	if (myReader.isInterrupted()) {
		return S_OK; // TODO: interrupt???
	}

	HRESULT res = decodeAttributes(pAttributes);
	if (res != S_OK) {
		return res;
	}

	if (decodeWideChars(pwchQName, cchQName, &myBuffer, &myBufferSize) < 0) {
		return E_FAIL;
	}

	if (myReader.processNamespaces()) {
		int count = 0;
		for (const char **a = (const char **)&myAttributes[0]; (*a != 0) && (*(a + 1) != 0); a += 2) {
			if (strncmp(*a, "xmlns:", 6) == 0) {
				if (count == 0) {
					myReader.myNamespaces.push_back(
						new std::map<std::string,std::string>(*myReader.myNamespaces.back())
					);
				}
				++count;
				const std::string id(*a + 6);
				const std::string reference(*(a + 1));
				(*myReader.myNamespaces.back())[id] = reference;
			}
		}
		if (count == 0) {
			myReader.myNamespaces.push_back(myReader.myNamespaces.back());
		} else {
			myReader.namespaceListChangedHandler();
		}
	}

	myReader.startElementHandler(myBuffer, (const char **)&myAttributes[0]);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE W32ContentHandler::endElement(const wchar_t *pwchNamespaceUri, int cchNamespaceUri,
		const wchar_t *pwchLocalName, int cchLocalName, const wchar_t *pwchQName, int cchQName) {

	if (myReader.isInterrupted()) {
		return S_OK; // TODO: interrupt???
	}

	if (decodeWideChars(pwchQName, cchQName, &myBuffer, &myBufferSize) < 0) {
		return E_FAIL;
	}
	myReader.endElementHandler(myBuffer);

	if (myReader.processNamespaces()) {
		shared_ptr<std::map<std::string,std::string> > oldMap = myReader.myNamespaces.back();
		myReader.myNamespaces.pop_back();
		if (myReader.myNamespaces.back() != oldMap) {
			myReader.namespaceListChangedHandler();
		}
	}
	return S_OK;
}


HRESULT STDMETHODCALLTYPE W32ContentHandler::skippedEntity(const wchar_t *pwchName, int cchName) {
	// TODO: handle skipped entity
	return S_OK;
}



HRESULT STDMETHODCALLTYPE W32ContentHandler::startDocument() {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE W32ContentHandler::endDocument() {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE W32ContentHandler::startPrefixMapping(const wchar_t *pwchPrefix, int cchPrefix, const wchar_t *pwchUri, int cchUri) {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE W32ContentHandler::endPrefixMapping(const wchar_t *pwchPrefix, int cchPrefix) {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE W32ContentHandler::ignorableWhitespace(const wchar_t *pwchChars, int cchChars) {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE W32ContentHandler::processingInstruction(const wchar_t *pwchTarget, int cchTarget,
		const wchar_t *pwchData, int cchData) {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE W32ContentHandler::putDocumentLocator(ISAXLocator *pLocator) {
	return S_OK;
}




HRESULT STDMETHODCALLTYPE W32ContentHandler::QueryInterface(REFIID riid, void **ppvObject) {
	if (ppvObject == 0) {
		return E_POINTER;
	}
	if (riid == __uuidof(IUnknown) || riid == __uuidof(ISAXContentHandler)) {
		*ppvObject = this;
		AddRef();
		return S_OK;
	}
	*ppvObject = 0;
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE W32ContentHandler::AddRef() {
	return (ULONG)InterlockedIncrement(&myRefs);
}

ULONG STDMETHODCALLTYPE W32ContentHandler::Release() {
	ULONG res = (ULONG) InterlockedDecrement(&myRefs);
	if (res == 0) {
		delete this;
	}
	return res;
}
